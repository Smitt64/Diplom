SELECT
student.surname AS `�������`,
student.name AS `���`,
student.patronymic AS `��������`,
groups.caption AS `������`
FROM
student
Inner Join groups ON groups.id = student.`group`
Inner Join specialty ON specialty.id = groups.special
WHERE
student.surname LIKE  :f AND
student.name LIKE  :i AND
student.patronymic LIKE  :o AND
groups.caption LIKE  :group AND
((year(now())-year(groups.intake)) <= specialty.during)
ORDER BY
student.surname ASC;