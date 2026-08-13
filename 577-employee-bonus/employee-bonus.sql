# Write your MySQL query statement below
SELECT
e1.name,b1.bonus
FROM Employee as e1
LEFT JOIN
Bonus as b1
ON
e1.empId = b1.empId
WHERE
b1.bonus<1000 OR
b1.bonus IS NULL