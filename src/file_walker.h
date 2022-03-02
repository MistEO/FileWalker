#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

class FileWalker
{
public:
    enum class Method
    {
        BFS,    // Breadth First Search
        DFS     // Depth First Search
    };
public:
    FileWalker(Method method, size_t threads_size = 16);
    ~FileWalker();

    std::vector<std::string> walk(const std::filesystem::path& path);

private:
    void bfs_working();
    void dfs_working();

    std::vector<std::string> m_result;
    std::mutex m_result_mutex;
    std::queue<std::filesystem::directory_entry> m_queue;
    std::mutex m_queue_mutex;

    bool m_exit = false;
    static constexpr size_t WorkingFlag = 114514;
    std::atomic<size_t> m_working_count = 0;
    std::condition_variable m_cond;
    std::vector<std::thread> m_threads;
};
