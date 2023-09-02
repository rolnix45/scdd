#pragma once

#include <prerequisites.hpp>

class ArenaAllocator {
public:
    explicit ArenaAllocator(size_t bytes);
    ArenaAllocator(ArenaAllocator const &aaloc) = delete;
    ArenaAllocator operator=(ArenaAllocator const &aaloc) = delete;
    ~ArenaAllocator();
    template <typename T>
    T* alloc() {
        void* offset = m_offset;
        m_offset += sizeof(T);
        return static_cast<T*>(offset);
    }
private:
    size_t m_size;
    std::byte* m_buffer;
    std::byte* m_offset;
};