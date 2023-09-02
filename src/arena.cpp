#include <arena.hpp>

#include <malloc.h>

ArenaAllocator::ArenaAllocator(size_t bytes) : m_size(std::move(bytes)) {
    m_buffer = static_cast<std::byte*>(malloc(m_size));
    m_offset = m_buffer;
}

ArenaAllocator::~ArenaAllocator() {
    free(m_buffer);
}

