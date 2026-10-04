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
        Task() = default;
        Task(const std::string &title, Priority priority, TaskState state, const std::optional<std::string> &tag)
            : title(title), priority(priority), state(state), tag(tag) {}
    };

    enum class TaskField
    {
        title,
        priority,
        state,
        tag
    };

    enum class Order
    {
        ascending,
        descending
    };

    struct SortOrder
    {
        TaskField field;
        Order order;
        SortOrder() = default;
        SortOrder(TaskField field, Order order) : field(field), order(order) {}
    };

    struct Filter
    {
        virtual ~Filter() = default;
        virtual bool matches(const Task &task) const = 0;
    };

    template <typename T>
    struct TypedFilter : public Filter
    {
        T value;
        T Task::*accessor;
        TypedFilter(T Task::*accessor, T value) : accessor(accessor), value(value) {}
        bool matches(const Task &task) const override
        {
            return task.*accessor == value;
        }
    };

    struct TaskTracker
    {
    private:
        std::vector<Task> allTasks;

        std::vector<SortOrder> sortOrders = {SortOrder(TaskField::priority, Order::descending)};
        std::vector<std::unique_ptr<Filter>> filters;

    public:
        const std::vector<Task> &getAllTasks() const
        {
            return allTasks;
        }
        std::vector<Task> getCurrentTaskList() const
        {
            std::vector<Task> currentTaskList = allTasks;
            for (const auto &filter : filters)
            {
                currentTaskList.erase(
                    std::remove_if(currentTaskList.begin(), currentTaskList.end(),
                                   [&](const Task &task)
                                   { return !filter->matches(task); }),
                    currentTaskList.end());
            }
            for (auto it = sortOrders.rbegin(); it != sortOrders.rend(); ++it)
            {
                const auto &sortOrder = *it;
                std::sort(currentTaskList.begin(), currentTaskList.end(),
                          [&](const Task &a, const Task &b)
                          {
                              switch (sortOrder.field)
                              {
                              case TaskField::title:
                                  return sortOrder.order == Order::ascending ? a.title < b.title : a.title > b.title;
                              case TaskField::priority:
                                  return sortOrder.order == Order::ascending ? a.priority < b.priority : a.priority > b.priority;
                              case TaskField::state:
                                  return sortOrder.order == Order::ascending ? a.state < b.state : a.state > b.state;
                              case TaskField::tag:
                                  return sortOrder.order == Order::ascending ? a.tag < b.tag : a.tag > b.tag;
                              default:
                                  return false;
                              }
                          });
            }
            return currentTaskList;
        }
        void addTask(const std::string &title, std::optional<Priority> priority = std::nullopt, const std::optional<std::string> &tag = std::nullopt)
        {
            auto trimmedTitle = title;
            // remove leading and trailing whitespace
            trimmedTitle.erase(trimmedTitle.begin(), std::find_if(trimmedTitle.begin(), trimmedTitle.end(), [](unsigned char ch)
                                                                  { return !std::isspace(ch); }));
            trimmedTitle.erase(std::find_if(trimmedTitle.rbegin(), trimmedTitle.rend(), [](unsigned char ch)
                                            { return !std::isspace(ch); })
                                   .base(),
                               trimmedTitle.end());
            if (trimmedTitle.empty())
            {
                throw std::invalid_argument("Task title cannot be empty");
            }
            allTasks.emplace_back(trimmedTitle, priority.value_or(Priority::medium), TaskState::open, tag);
        }
        template <typename... T>
        void setFilters(TypedFilter<T>... filters)
        {
            this->filters.clear();
            (this->filters.emplace_back(std::make_unique<TypedFilter<T>>(std::move(filters))), ...);
        }
        template <typename... S>
        void setSortOrders(S... sortOrders)
        {
            this->sortOrders = {sortOrders...};
        }
        void setTaskCompleted(const std::string &title)
        {
            for (auto &task : allTasks)
            {
                if (task.title == title)
                {
                    task.state = TaskState::done;
                    break;
                }
            }
        }
        void removeTask(const std::string &title)
        {
            allTasks.erase(std::remove_if(allTasks.begin(), allTasks.end(),
                                          [&](const Task &task)
                                          { return task.title == title; }),
                           allTasks.end());
        }
    };

    class TestTaskTracker : public ITestTaskTracker
    {
        TaskTracker tracker;
        std::string lastErrorMessage;

        void assert_exact_tasks(const std::vector<ListingAssertionByTitle> &expectedTitles)
        {
            ASSERT_EQ(tracker.getCurrentTaskList().size(), expectedTitles.size());
            for (size_t i = 0; i < tracker.getCurrentTaskList().size(); ++i)
            {
                ASSERT_EQ(tracker.getCurrentTaskList()[i].title, expectedTitles[i].title);
            }
        }

    public:
        void an_empty_task_tracker()
        {
            tracker = {};
        }
        void i_have_added_the_following_tasks(const std::vector<TaskDef> &rows)
        {
            for (const auto &[title, priority, tag] : rows)
            {
                tracker.addTask(title, priority, tag);
            }
        }
        void i_list_open_tasks_with_tag(const std::string &tag)
        {
            tracker.setFilters(TypedFilter<TaskState>(&Task::state, TaskState::open),
                               TypedFilter<std::optional<std::string>>(&Task::tag, tag));
        }
        void the_listed_tasks_should_be_exactly(const std::vector<ListingAssertionByTitle> &expectedTitles)
        {
            assert_exact_tasks(expectedTitles);
        }
        void i_list_open_tasks_with_priority(Priority priority)
        {
            tracker.setFilters(TypedFilter<Priority>(&Task::priority, priority));
        }
        void i_complete_the_task(const std::string &task)
        {
            tracker.setTaskCompleted(task);
        }
        void i_list_done_tasks()
        {
            tracker.setFilters(TypedFilter<TaskState>(&Task::state, TaskState::done));
        }
        void i_remove_the_task(const std::string &task)
        {
            tracker.removeTask(task);
        }
        void i_list_open_tasks()
        {
            tracker.setFilters(TypedFilter<TaskState>(&Task::state, TaskState::open));
        }
        void i_add_a_task_titled(const std::string &title)
        {
            tracker.addTask(title);
        }
        void the_task_should_have_priority(const std::string &task, Priority priority)
        {
            for (const auto &taskItem : tracker.getAllTasks())
            {
                if (taskItem.title == task)
                {
                    ASSERT_EQ(taskItem.priority, priority);
                    return;
                }
            }
            FAIL() << "Task not found: " << task;
        }
        void the_operation_should_fail_with(const std::string &errorMessage)
        {
            ASSERT_EQ(lastErrorMessage, errorMessage);
        }
        void assert_tasks_with_state(long count, TaskState state)
        {
            long actualCount = 0;
            for (const auto &taskItem : tracker.getAllTasks())
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
        void around_step(const StepContext &context, const std::function<void()> &step)
        {
            ITestTaskTracker::around_step(context,
                                          [&]()
                                          {
                try
                {
                    step();
                    lastErrorMessage = "";
                }
                catch (const std::exception &e)
                {
                    if (context.next() && context.next().value().method == StepMethod::the_operation_should_fail_with)
                    {
                        lastErrorMessage = e.what();
                    }
                    else
                    {
                        throw;
                    }
                } });
        }
    };
} // namespace nTestTaskTracker