SELECT
groups.caption AS `Название`,
groups.intake AS `Поступление`,
Count(student.id) AS `Учащихся`,
specialty.caption AS `Специальность`
FROM
groups
Inner Join student ON groups.id = student.`group`
Inner Join specialty ON specialty.id = groups.special
where ((year(now())-year(groups.intake)) <= specialty.during)
GROUP BY
groups.caption,
groups.intake,
specialty.caption
ORDER BY groups.caption;