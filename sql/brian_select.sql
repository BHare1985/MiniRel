

CREATE TABLE stars(starid integer, real_name char(20), plays char(12), soapid integer);


INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (1, 'Name 1', 'Plays 1', 1);
INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (2, 'Name 2', 'Plays 2', 2);
INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (3, 'Name 3', 'Plays 3', 3);
INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (4, 'Name 4', 'Plays 4', 4);

-- Check unconditional ,no index,
SELECT * FROM stars;
SELECT stars.soapid FROM stars;
SELECT stars.real_name, stars.soapid FROM stars;

-- Check conditional, no index, non-equality
SELECT * FROM stars WHERE stars.starid > 0;
SELECT stars.soapid FROM stars WHERE stars.starid > 0;
SELECT stars.real_name, stars.soapid FROM stars WHERE stars.starid > 0;

-- Check conditional, no index, equality
SELECT * FROM stars WHERE stars.starid = 1;
SELECT stars.soapid FROM stars WHERE stars.starid = 1;
SELECT stars.real_name, stars.soapid FROM stars WHERE stars.starid = 1;
SELECT * FROM stars WHERE stars.starid != 1;
SELECT stars.soapid FROM stars WHERE stars.starid != 1;
SELECT stars.real_name, stars.soapid FROM stars WHERE stars.starid != 1;


CREATE INDEX stars(starid);

-- Check unconditional, index,
SELECT * FROM stars;
SELECT stars.soapid FROM stars;
SELECT stars.real_name, stars.soapid FROM stars;

-- Check conditional, index, non-equality
SELECT * FROM stars WHERE stars.starid > 0;
SELECT stars.soapid FROM stars WHERE stars.starid > 0;
SELECT stars.real_name, stars.soapid FROM stars WHERE stars.starid > 0;

-- Check all non equalities
SELECT stars.soapid FROM stars WHERE stars.starid > 0;
SELECT stars.soapid FROM stars WHERE stars.starid >= 1;
SELECT stars.soapid FROM stars WHERE stars.starid < 5;
SELECT stars.soapid FROM stars WHERE stars.starid <= 4;

-- Check conditional, index, equality
SELECT * FROM stars WHERE stars.starid = 1;
SELECT stars.soapid FROM stars WHERE stars.starid = 1;
SELECT stars.real_name, stars.soapid FROM stars WHERE stars.starid = 1;
SELECT * FROM stars WHERE stars.starid != 1;
SELECT stars.soapid FROM stars WHERE stars.starid != 1;
SELECT stars.real_name, stars.soapid FROM stars WHERE stars.starid != 1;


DROP TABLE stars;
