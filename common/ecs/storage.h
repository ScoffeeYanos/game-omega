#pragma once
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
struct Entity
{
    uint32_t id{};
    bool operator==(const Entity &o) const
    {
        return id==o.id;
    }
};
class Storage
{
public:
    Entity create_entity()
    {
        uint32_t id;
        if(!free_ids_.empty())
        {
            id = free_ids_.back();
            free_ids_.pop_back();
        }
        else
        {
            id=++next_id_;
        }
        return Entity{id};
    }
    void destroy_entity(Entity e)
    {
        for (auto& [type, base_ptr] : storages_)
        {
            if (!base_ptr) continue;
            base_ptr->remove_entity(e.id);
        }
        free_ids_.push_back(e.id);
    }
    template<typename C, typename... Args>
    bool emplace(Entity e,Args&&... args)
    {
        auto& s = storage<C>();
        if (s.sparse.size() <= e.id)
            s.sparse.resize(e.id + 1, 0);
        if (s.sparse[e.id]!=0) return false;
        const auto idx = static_cast<uint32_t>(s.dense_data.size());
        s.dense_data.emplace_back(std::forward<Args>(args)...);
        s.dense_entities.emplace_back(e.id);
        s.sparse[e.id]=idx+1;
        return true;
    }
    template<typename C>
    bool has(Entity e)
    {
        auto&s = storage<C>();
        return e.id < s.sparse.size() && s.sparse[e.id]!=0;
    }
    template<typename C>
    C& get(Entity e)
    {
        auto&s = storage<C>();
        return s.dense_data[s.sparse[e.id]-1];
    }
    template<typename C>
    bool remove(Entity e)
    {
        auto&s = storage<C>();
        if (e.id >= s.sparse.size() || s.sparse[e.id]==0) return false; // no Entity e saved
        auto idx = s.sparse[e.id]-1;
        auto last = static_cast<uint32_t>(s.dense_data.size() - 1);
        if(idx != last)//move to delete item do last
        {
            const uint32_t moved_entity = s.dense_entities[last];
            s.dense_data[idx]     = std::move(s.dense_data[last]);
            s.dense_entities[idx] = moved_entity;
            s.sparse[moved_entity] = idx + 1;
        }
        //delete last
        s.dense_data.pop_back();
        s.dense_entities.pop_back();
        s.sparse[e.id]=0;
        return true;
    }

private:
    uint32_t next_id_=0;
    struct IContainer
    {
        virtual ~IContainer() = default;
        virtual void remove_entity(uint32_t id) = 0;
    };
    template<typename C> struct Container : IContainer
    {
        std::vector<C>       dense_data; //data
        std::vector<uint32_t> dense_entities; // data->id
        std::vector<uint32_t> sparse; // id->data

        void remove_entity(uint32_t id) override
        {
            if (id >= sparse.size() || sparse[id] == 0) return;

            auto idx  = sparse[id] - 1;
            auto last = static_cast<uint32_t>(dense_data.size() - 1);

            if (idx != last)
            {
                uint32_t moved_entity = dense_entities[last];
                dense_data[idx] = std::move(dense_data[last]);
                dense_entities[idx] = moved_entity;
                sparse[moved_entity] = idx + 1;
            }

            dense_data.pop_back();
            dense_entities.pop_back();
            sparse[id] = 0;
        }
    };
    template<typename C> Container<C> &storage()
    {
        auto key=std::type_index(typeid(C));
        auto& ptr = storages_[key];
        if (!ptr) ptr = std::make_unique<Container<C>>();
        return *static_cast<Container<C>*>(ptr.get());
    }
    std::unordered_map<std::type_index,std::unique_ptr<IContainer>> storages_;
    std::vector<uint32_t> free_ids_;
};