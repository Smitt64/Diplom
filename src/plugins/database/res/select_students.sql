SELECT
student.surname AS `Фамилмя`,
student.name AS `Имя`,
student.patronymic AS `Отчество`,
groups.caption AS `Группа`
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