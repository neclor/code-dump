DELIMITER $$

DROP TRIGGER IF EXISTS trigger_specializes_insert $$
CREATE TRIGGER trigger_specializes_insert BEFORE INSERT ON specializes
FOR EACH ROW
BEGIN
    IF genre_create_cycle(NEW.genre, NEW.subgenre) THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cycle detected: This specialize would create a circular dependency.';
    END IF;
END $$

DROP TRIGGER IF EXISTS trigger_specializes_update $$
CREATE TRIGGER trigger_specializes_update BEFORE UPDATE ON specializes
FOR EACH ROW
BEGIN
    IF genre_create_cycle(NEW.genre, NEW.subgenre) THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Cycle detected: This specialize would create a circular dependency.';
    END IF;
END $$

DROP TRIGGER IF EXISTS trigger_event_insert $$
CREATE TRIGGER trigger_event_insert BEFORE INSERT ON event
FOR EACH ROW
BEGIN
    IF NEW.date <= CURDATE() THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Event date must be in the future.';
    END IF;

    CALL check_event_field_dependencies(NEW.event_planner, NEW.manager, NEW.dj, NEW.location, NEW.rental_fee);
    CALL check_employee_availability(NEW.dj, NEW.date, NEW.id);
    CALL check_employee_availability(NEW.event_planner, NEW.date, NEW.id);
    CALL check_location_availability(NEW.location, NEW.date, NEW.id);
END $$

DROP TRIGGER IF EXISTS trigger_event_update $$
CREATE TRIGGER trigger_event_update BEFORE UPDATE ON event
FOR EACH ROW
BEGIN
    IF OLD.date <= CURDATE() AND NEW.date <= CURDATE() THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Only future events can be edited.';
    END IF;

    CALL check_event_field_dependencies(NEW.event_planner, NEW.manager, NEW.dj, NEW.location, NEW.rental_fee);
    CALL check_employee_availability(NEW.dj, NEW.date, NEW.id);
    CALL check_employee_availability(NEW.event_planner, NEW.date, NEW.id);
    CALL check_location_availability(NEW.location, NEW.date, NEW.id);
END $$

DROP TRIGGER IF EXISTS trigger_supervision_insert $$
CREATE TRIGGER trigger_supervision_insert BEFORE INSERT ON supervision
FOR EACH ROW
BEGIN
    IF NEW.supervisor_id = NEW.employee_id THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'An employee cannot supervise themselves.';
    END IF;
END $$

DROP TRIGGER IF EXISTS trigger_supervision_update $$
CREATE TRIGGER trigger_supervision_update BEFORE UPDATE ON supervision
FOR EACH ROW
BEGIN
    IF NEW.supervisor_id = NEW.employee_id THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'An employee cannot supervise themselves.';
    END IF;
END $$

DROP TRIGGER IF EXISTS before_request_insert $$
CREATE TRIGGER before_request_insert BEFORE INSERT ON request
FOR EACH ROW
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM event
        WHERE id = NEW.event_id AND event_planner IS NOT NULL
    ) THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'There must be an event planner before accepting any request.';
    END IF;
END $$

DELIMITER ;
