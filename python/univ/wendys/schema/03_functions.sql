DELIMITER $$

DROP FUNCTION IF EXISTS calculate_event_cost $$
CREATE FUNCTION calculate_event_cost(id INT)
RETURNS DECIMAL(10, 2)
READS SQL DATA
BEGIN
    DECLARE total_price DECIMAL(10, 2);
    DECLARE base_cost DECIMAL(10, 2) DEFAULT 1500.00;

    SELECT IFNULL(SUM(price), 0) INTO total_price
    FROM request
    WHERE request.event_id = id;

    RETURN ROUND(base_cost + total_price + (total_price * 0.05), 2);
END $$

DROP FUNCTION IF EXISTS cd_get_total_song_duration $$
CREATE FUNCTION cd_get_total_song_duration(cd_number INT)
RETURNS TIME
READS SQL DATA
BEGIN
    DECLARE total_song_duration TIME;

    SELECT IFNULL(SEC_TO_TIME(SUM(TIME_TO_SEC(song.duration))), '00:00:00') INTO total_song_duration
    FROM song
    WHERE song.cd_number = cd_number;

    RETURN total_song_duration;
END $$

DROP FUNCTION IF EXISTS cd_get_min_song_duration $$
CREATE FUNCTION cd_get_min_song_duration(cd_number INT)
RETURNS TIME
READS SQL DATA
BEGIN
    DECLARE min_song_duration TIME;

    SELECT IFNULL(MIN(song.duration), '00:00:00') INTO min_song_duration
    FROM song
    WHERE song.cd_number = cd_number;

    RETURN min_song_duration;
END $$

DROP FUNCTION IF EXISTS cd_get_max_song_duration $$
CREATE FUNCTION cd_get_max_song_duration(cd_number INT)
RETURNS TIME
READS SQL DATA
BEGIN
    DECLARE max_song_duration TIME;

    SELECT IFNULL(MAX(song.duration), '00:00:00') INTO max_song_duration
    FROM song
    WHERE song.cd_number = cd_number;

    RETURN max_song_duration;
END $$

DROP FUNCTION IF EXISTS cd_get_average_song_duration $$
CREATE FUNCTION cd_get_average_song_duration(cd_number INT)
RETURNS TIME
READS SQL DATA
BEGIN
    DECLARE avg_song_duration TIME;

    SELECT IFNULL(SEC_TO_TIME(AVG(TIME_TO_SEC(song.duration))), '00:00:00') INTO avg_song_duration
    FROM song
    WHERE song.cd_number = cd_number;

    RETURN avg_song_duration;
END $$

DROP FUNCTION IF EXISTS cd_playlist_count $$
CREATE FUNCTION cd_playlist_count(cd_number INT)
RETURNS INT
READS SQL DATA
BEGIN
    DECLARE occurrence_count INT;

    SELECT COUNT(*) INTO occurrence_count
    FROM contains
    WHERE contains.cd_number = cd_number;

    RETURN occurrence_count;
END $$

DELIMITER ;
