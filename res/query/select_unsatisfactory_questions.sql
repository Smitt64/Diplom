SELECT
test_results.report
FROM
test_results
WHERE
test_results.subject = :subject AND
test_results.student = :student AND
test_results.score <  '3' AND
(test_results.`data` > last_year(Now()) + interval 1 day - interval 1 month);