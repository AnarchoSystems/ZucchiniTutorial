Feature: Filtering tasks
  To focus on what matters
  I want to filter tasks by tag, priority, and status

  Background:
    Given an empty task tracker
    And I have added the following tasks:
      | Title         | Priority | Tag  |
      | Fix login bug | high     | work |
      | Water plants  | low      | home |
      | Review PR     | high     | work |

  Scenario: Filter by tag
    When I list open tasks with tag "work"
    Then the listed tasks should be exactly:
      | Title         |
      | Fix login bug |
      | Review PR     |

  Scenario: Filter by priority
    When I list open tasks with priority "high"
    Then the listed tasks should be exactly:
      | Title         |
      | Fix login bug |
      | Review PR     |

  Scenario: Filtering excludes completed tasks by default
    Given I complete the task "Fix login bug"
    When I list open tasks with tag "work"
    Then the listed tasks should be exactly:
      | Title     |
      | Review PR |

  Scenario: Explicitly list completed tasks
    Given I complete the task "Water plants"
    When I list done tasks
    Then the listed tasks should be exactly:
      | Title        |
      | Water plants |

  Scenario: Removing a task
    When I remove the task "Review PR"
    And I list open tasks
    Then the listed tasks should be exactly:
      | Title         |
      | Fix login bug |
      | Water plants  |