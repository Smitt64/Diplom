SELECT
student.id,
concat_ws(' ',student.surname,student.name,student.patronymic) AS `�������`
FROM
student
Inner Join groups ON groups.id = student.`group`
WHERE
groups.caption =  :param
ORDER BY
student.surname ASC;