SELECT
groups.caption,
groups.cource
FROM
groups
Inner Join specialty ON specialty.id = groups.special
