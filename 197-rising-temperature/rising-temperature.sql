# Write your MySQL query statement below
# we will compare with previous dates only,i.e. with consecutive dates. for eg. 5jan temp. will be comapred with 4jan temp. only not any other date that is not present in consecutive order
#INNER JOIN- returns the commom part of the intersection of 2 tables
# DATEDIFF is the fn. that tells or returns the diff btw 2 dates in no.of days
SELECT w1.id
FROM Weather as w1
INNER JOIN Weather as w2
WHERE DATEDIFF(w1.recordDate , w2.recordDate) = 1 #SUBDATE(w2.recordDate,1)=(w1.recordDate)
AND
w1.temperature>w2.temperature