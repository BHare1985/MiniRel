-- NOTE:
-- This file performs sanity checks for INSERT and SELECT
-- Since you are testing INSERT using SELECT, you will have
-- to think carefully about the source of your bug.


-- create relations
CREATE TABLE stars(starid integer, real_name char(20), 
                   plays char(12), soapid integer);

--
-- Insert Test w/o indices
-- 

-- Insert with attributes in order:
INSERT INTO stars(starid, real_name, plays, soapid) 
	VALUES (1, 'Graham, Laura', 'Lorelei', 100);

-- Run a simple SELECT
SELECT * FROM stars;

-- Insert with attributes out of order:
INSERT INTO stars (real_name, soapid, starid, plays) 
	VALUES ('Bonarrigo, Laura', 3, 101, 'Cassie');

-- Run a simple SELECT
SELECT * FROM stars;

-- Test relcat and attrcat are up to date
SELECT * FROM relcat;
SELECT * FROM attrcat;

DROP TABLE stars;

