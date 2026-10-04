#include "ITestTaskTracker.h"

namespace nTestTaskTracker
{

    class TestTaskTracker : public ITestTaskTracker
    {
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
        void the_listed_tasks_should_be_exactly(const std::vector<ListingAssertionByTitle> &rows)
        {
            // TODO
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
            // TODO
        }
        void i_try_to_add_a_task_titled(const std::string &title)
        {
            // TODO
        }
        void the_operation_should_fail_with(const std::string &errorMessage)
        {
            // TODO
        }
        void assert_tasks_with_state(long count, TaskState state)
        {
            // TODO
        }
        void the_tasks_should_be_ordered_as(const std::vector<ListingAssertionByTitle> &rows)
        {
            // TODO
        }
    };
} // namespace nTestTaskTracker