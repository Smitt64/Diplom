SELECT
groups.caption AS `��������`,
groups.intake AS `�����������`,
Count(student.id) AS `��������`,
specialty.caption AS `�������������`
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