SELECT
t.tests_count
FROM
testing_state AS t
WHERE
((Now() between  t.date and t.end_date) and
((t.student = :s_param) AND
(t.test = md5(:test))));
