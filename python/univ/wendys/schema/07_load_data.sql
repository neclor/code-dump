LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/CD.csv'             INTO TABLE cd             FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Client.csv'         INTO TABLE client         FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Employee.csv'       INTO TABLE employee       FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/DJ.csv'             INTO TABLE dj             FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/EventPlanner.csv'   INTO TABLE eventplanner   FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Genre.csv'          INTO TABLE genre          FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Location.csv'       INTO TABLE location       FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Manager.csv'        INTO TABLE manager        FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Playlist.csv'       INTO TABLE playlist       FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Song.csv'           INTO TABLE song           FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Contains.csv'       INTO TABLE contains       FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Specialization.csv' INTO TABLE specialization FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Specializes.csv'    INTO TABLE specializes    FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Theme.csv'          INTO TABLE theme          FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Suitablefor.csv'    INTO TABLE suitablefor    FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;

LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Supervision.csv'    INTO TABLE supervision    FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES
(supervisor_id, employee_id);

LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Event.csv'          INTO TABLE event          FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES
(id, name, date, @v_desc, client, manager, event_planner, dj, theme, type, @v_loc, @v_fee, @v_playlist)
SET
    description = NULLIF(@v_desc,      ''),
    location    = NULLIF(@v_loc,       ''),
    rental_fee  = NULLIF(@v_fee,       ''),
    playlist    = NULLIF(@v_playlist,  '');

LOAD DATA INFILE '/docker-entrypoint-initdb.d/data/Request.csv'        INTO TABLE request        FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' LINES TERMINATED BY '\n' IGNORE 1 LINES;
