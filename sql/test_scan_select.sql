CREATE TABLE COLORS(num integer, val integer, color char(80));

INSERT INTO COLORS (num, val, color) VALUES(1, 100, 'red');
INSERT INTO COLORS (num, val, color) VALUES(2, 100, 'orange');
INSERT INTO COLORS (num, val, color) VALUES(8, 400, 'periwinkle');
INSERT INTO COLORS (num, val, color) VALUES(3, 200, 'yellow');
INSERT INTO COLORS (num, val, color) VALUES(4, 200, 'green');
INSERT INTO COLORS (num, val, color) VALUES(5, 200, 'blue');
INSERT INTO COLORS (num, val, color) VALUES(6, 300, 'purple');
INSERT INTO COLORS (num, val, color) VALUES(7, 300, 'violet');

--Testing where there is no index, but there is an equals sign
SELECT * FROM COLORS WHERE COLORS.val = 100; --should select many
SELECT * FROM COLORS WHERE COLORS.val = 400; --should select 1
SELECT * FROM COLORS WHERE COLORS.val = 700; --should select none

--Testing where there is no index, but there is a non equals sign
SELECT * FROM COLORS WHERE COLORS.val > 100; --should select many
SELECT * FROM COLORS WHERE COLORS.val > 300; --should select 1
SELECT * FROM COLORS WHERE COLORS.val < 400; --should select many
SELECT * FROM COLORS WHERE COLORS.val > 700; --should select none

CREATE INDEX COLORS (val); --creating an index

--Testing where there is an index, but there is a non equals sign
SELECT * FROM COLORS WHERE COLORS.val > 100; --should select many
SELECT * FROM COLORS WHERE COLORS.val > 300; --should select 1
SELECT * FROM COLORS WHERE COLORS.val < 400; --should select many
SELECT * FROM COLORS WHERE COLORS.val > 700; --should select none

