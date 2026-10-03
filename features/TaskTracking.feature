Feature: Task tracking
  As a busy user
  I want to track my tasks
  So that I don't forget what matters

  Background:

    Given an empty task tracker

  Scenario: Add a simple task
    When I add a task titled "Buy groceries"
    Then the tracker should have 1 open task
    And the task "Buy groceries" should have priority "medium"

  Scenario: Task titles cannot be blank
    When I try to add a task titled "   "
    Then the operation should fail with "Task title cannot be empty"

  Scenario: Complete a task
    And I have added the following tasks:
      | Title        |
      | Write report |
    When I complete the task "Write report"
    Then the tracker should have 0 open tasks
    And the tracker should have 1 done task

  Scenario: Tasks are ordered by priority
    And I have added the following tasks:
      | Title  | Priority |
      | Low    | low      |
      | High   | high     |
      | Medium | medium   |
    When I list open tasks
    Then the tasks should be ordered as:
      | Title  |
      | High   |
      | Medium |
      | Low    |