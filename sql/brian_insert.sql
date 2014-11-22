--
-- test Insert with and without indices
--

-- create relations

CREATE TABLE stars(starid integer, real_name char(20), plays char(12), soapid integer);

-- Normal insert
INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (100, 'Posey, Parker', 'Tess', 6);
-- Omitting column
INSERT INTO stars(starid, real_name, plays)  VALUES (100, 'Posey, Parker', 'Tess');


-- Out of order Test
INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (1, 'Name 1', 'Plays 1', 1);
INSERT INTO stars(real_name, plays, soapid, starid)  VALUES ('Name 2', 'Plays 2', 2, 2);
INSERT INTO stars(plays, soapid, starid, real_name)  VALUES ('Plays 3', 3, 3, 'Name 3');
INSERT INTO stars(soapid, starid, real_name, plays)  VALUES (4, 4, 'Name 4', 'Plays 4');

	
SELECT * FROM stars;
DROP TABLE stars;
