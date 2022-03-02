#include "file_walker.h"

#include <iostream>
#include <functional>

FileWalker::FileWalker(Method method, size_t threads_size)
{
    decltype(&FileWalker::bfs_working) func = nullptr;
    switch (method) {
    case Method::BFS:
        func = &FileWalker::bfs_working;
        break;
    case Method::DFS:
        func = &FileWalker::dfs_working;
        break;
    default:
        throw std::runtime_error("Invalid walk method");
        break;
    }

    for (size_t i = 0; i < threads_size; ++i) {
        m_threads.emplace_back(func, this);
    }
}

FileWalker::~FileWalker()
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_exit = true;
    }
    m_cond.notify_all();

    for (auto&& thread : m_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

std::vector<std::string> FileWalker::walk(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path)) {
        return {};
    }
    if (!std::filesystem::is_directory(path)) {
        return { path.string() };
    }

    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_queue.emplace(path);
    }
    m_working_count = WorkingFlag;
    m_cond.notify_one();

    while (m_working_count != 0) { // wait for finished
        ;
    }

    return m_result;
}

void FileWalker::bfs_working()
{
    while (!m_exit) {
        std::filesystem::directory_entry dir_entry;
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_cond.wait(lock, [this]() ->bool {return m_exit || !m_queue.empty();});

            if (m_working_count == WorkingFlag) {
                m_working_count = 1;
            }
            else {
                ++m_working_count;
            }
            if (m_exit || m_queue.empty()) {
                --m_working_count;
                return;
            }
            dir_entry = std::move(m_queue.front());
            m_queue.pop();
        }

        for (const auto& entry : std::filesystem::directory_iterator(dir_entry.path())) {
            {
                std::unique_lock<std::mutex> lock(m_result_mutex);
                m_result.emplace_back(entry.path().string());
            }
            if (entry.is_directory()) {
                {
                    std::unique_lock<std::mutex> lock(m_queue_mutex);
                    m_queue.emplace(entry);
                }
                m_cond.notify_one();
            }
        }
        --m_working_count;
    }
}

void FileWalker::dfs_working()
{}
