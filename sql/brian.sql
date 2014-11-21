--
-- test Insert with and without indices
--

-- create relations

CREATE TABLE stars(starid integer, real_name char(20), plays char(12), soapid integer);

-- insert with attributes in order:
INSERT INTO stars(starid, real_name, plays, soapid)  VALUES (100, 'Posey, Parker', 'Tess', 6);


DROP TABLE stars;
