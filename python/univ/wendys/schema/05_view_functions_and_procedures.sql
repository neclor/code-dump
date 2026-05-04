DELIMITER $$

DROP FUNCTION IF EXISTS genre_create_cycle $$
CREATE FUNCTION genre_create_cycle(new_genre VARCHAR(255), new_subgenre VARCHAR(255))
RETURNS BOOLEAN
READS SQL DATA
BEGIN
    RETURN EXISTS (
        SELECT 1
        FROM specializes_transitive_closure
        WHERE genre = new_subgenre AND subgenre = new_genre
    );
END $$

DROP PROCEDURE IF EXISTS check_event_field_dependencies $$
CREATE PROCEDURE check_event_field_dependencies(
    IN p_event_planner INT,
    IN p_manager       INT,
    IN p_dj            INT,
    IN p_location      INT,
    IN p_rental_fee    DECIMAL(10,2)
)
BEGIN
    IF p_event_planner IS NOT NULL AND p_manager IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cannot assign event_planner without a manager.';
    END IF;
    IF p_dj IS NOT NULL AND p_manager IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cannot assign dj without a manager.';
    END IF;
    IF p_location IS NOT NULL AND p_event_planner IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cannot assign location without an event_planner.';
    END IF;
    IF p_rental_fee IS NOT NULL AND p_location IS NULL THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cannot assign rental_fee without a location.';
    END IF;
END $$

DROP PROCEDURE IF EXISTS check_employee_availability $$
CREATE PROCEDURE check_employee_availability(
    IN employee_id INT,
    IN event_date  DATE,
    IN current_id  INT
)
BEGIN
    IF employee_id IS NOT NULL THEN
        IF EXISTS (
            SELECT 1 FROM event
            WHERE date = event_date AND id != current_id AND (dj = employee_id OR event_planner = employee_id)
        ) THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'Employee member is already booked for this date.';
        END IF;
    END IF;
END $$

DROP PROCEDURE IF EXISTS check_location_availability $$
CREATE PROCEDURE check_location_availability(
    IN location_id INT,
    IN event_date  DATE,
    IN current_id  INT
)
BEGIN
    IF location_id IS NOT NULL THEN
        IF EXISTS (
            SELECT 1 FROM event
            WHERE date = event_date AND id != current_id AND location = location_id
        ) THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'Location is already booked for this date.';
        END IF;
    END IF;
END $$

DELIMITER ;
