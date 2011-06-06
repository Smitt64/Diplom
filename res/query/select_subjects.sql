SELECT DISTINCT
subject.id,
subject.subject
FROM
subject, subject_specialty, specialty
where
(subject.id = subject_specialty.subject 
AND
specialty.id = subject_specialty.specialty)
