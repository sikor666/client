#pragma once

#include <memory>

namespace execq {
/**
 * @class IExecutionStream
 * @brief High-level interface that provides access to stream-based tasks execution.
 *
 * @discussion ExecutionStream executes tasks simultaneously in the most efficient way
 * on all available Pool threads every time the thread lacks of work.
 *
 * @discussion Stream could be used when number of tasks is unknown.
 * It could be such cases like filesystem traverse: number of files is not determined, but you want to process
 * all of them in the most efficient way.
 */
class IExecutionStream
{
public:
    virtual ~IExecutionStream() = default;

    /**
     * @brief Starts execution stream.
     * Each time when thread in pool becomes free, execution stream will be prompted of next task to execute.
     */
    virtual void start() = 0;

    /**
     * @brief Stops execution stream.
     * Execution stream will not be prompted of next tasks to execute until 'start' is called.
     * All tasks being executed during stop will normally continue.
     */
    virtual void stop() = 0;
};
} // namespace execq
