#pragma once
#include <gtest/gtest.h>
#include "ITestTaskTracker.h"

namespace nTestTaskTracker
{

    struct Task
    {
        std::string title;
        Priority priority;
        TaskState state;
        std::optional<std::string> tag;
    };

    class TestTaskTracker : public ITestTaskTracker
    {
        std::vector<Task> allTasks;
        std::vector<Task> currentTaskList;

        void assert_exact_tasks(const std::vector<ListingAssertionByTitle> & expectedTitles)
        {
            ASSERT_EQ(currentTaskList.size(), expectedTitles.size());
            for (size_t i = 0; i < currentTaskList.size(); ++i)
            {
                ASSERT_EQ(currentTaskList[i].title, expectedTitles[i].title);
            }
        }

    public:
        void an_empty_task_tracker()
        {
            // TODO
        }
        void i_have_added_the_following_tasks(const std::vector<TaskDef> &rows)
        {
            // TODO
        }
        void i_list_open_tasks_with_tag(const std::string &tag)
        {
            // TODO
        }
        void the_listed_tasks_should_be_exactly(const std::vector<ListingAssertionByTitle> &expectedTitles)
        {
            assert_exact_tasks(expectedTitles);
        }
        void i_list_open_tasks_with_priority(Priority priority)
        {
            // TODO
        }
        void i_complete_the_task(const std::string &task)
        {
            // TODO
        }
        void i_list_done_tasks()
        {
            // TODO
        }
        void i_remove_the_task(const std::string &task)
        {
            // TODO
        }
        void i_list_open_tasks()
        {
            // TODO
        }
        void i_add_a_task_titled(const std::string &title)
        {
            // TODO
        }
        void the_task_should_have_priority(const std::string &task, Priority priority)
        {
            for (const auto &taskItem : allTasks)
            {
                if (taskItem.title == task)
                {
                    ASSERT_EQ(taskItem.priority, priority);
                    return;
                }
            }
            FAIL() << "Task not found: " << task;
        }
        void i_try_to_add_a_task_titled(const std::string &title)
        {
            // TODO
        }
        void the_operation_should_fail_with(const std::string &errorMessage)
        {
            // TODO ???
        }
        void assert_tasks_with_state(long count, TaskState state)
        {
            long actualCount = 0;
            for (const auto &taskItem : allTasks)
            {
                if (taskItem.state == state)
                {
                    ++actualCount;
                }
            }
            ASSERT_EQ(actualCount, count);
        }
        void the_tasks_should_be_ordered_as(const std::vector<ListingAssertionByTitle> &rows)
        {
            assert_exact_tasks(rows);
        }
    };
} // namespace nTestTaskTracker