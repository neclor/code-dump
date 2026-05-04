DROP VIEW IF EXISTS specializes_transitive_closure;
CREATE VIEW specializes_transitive_closure AS
    WITH RECURSIVE specializes_transitive(genre, subgenre) AS (
        SELECT genre, subgenre FROM specializes
        UNION ALL
        SELECT st.genre, s.subgenre
        FROM specializes_transitive st, specializes s
        WHERE st.subgenre = s.genre
    )
    SELECT * FROM specializes_transitive;

DROP VIEW IF EXISTS genre_with_subgenres;
CREATE VIEW genre_with_subgenres(genre, subgenres) AS
    SELECT
        genre.name,
        (
            SELECT GROUP_CONCAT(subgenre ORDER BY subgenre SEPARATOR ', ')
            FROM specializes_transitive_closure stc
            WHERE genre.name = stc.genre
        ) AS subgenres
    FROM genre;

DROP VIEW IF EXISTS event_dashboard;
CREATE VIEW event_dashboard AS
    SELECT
        event.id,
        event.name,
        event.date,
        CASE
            WHEN event.date < CURDATE() THEN 'PAST'
            WHEN event.date = CURDATE() THEN 'TODAY'
            ELSE 'FUTURE'
        END AS status,
        (SELECT COUNT(*) FROM request WHERE request.event_id = event.id) AS nb_request,
        calculate_event_cost(event.id) AS cost
    FROM event
    ORDER BY event.date DESC, event.name ASC;

DROP VIEW IF EXISTS cd_statistics;
CREATE VIEW cd_statistics AS
    SELECT
        cd.cd_number,
        cd_get_total_song_duration(cd.cd_number) AS total_duration,
        cd_get_min_song_duration(cd.cd_number) AS min_duration,
        cd_get_max_song_duration(cd.cd_number) AS max_duration,
        cd_get_average_song_duration(cd.cd_number) AS average_duration,
        cd_playlist_count(cd.cd_number) AS playlist_count,
        (
            SELECT GROUP_CONCAT(all_genres.genre ORDER BY all_genres.genre SEPARATOR ', ')
            FROM (
                SELECT song.genre
                FROM song
                WHERE song.cd_number = cd.cd_number
                UNION
                SELECT stc.genre
                FROM song
                JOIN specializes_transitive_closure stc ON stc.subgenre = song.genre
                WHERE song.cd_number = cd.cd_number
            ) AS all_genres
        ) AS genres
    FROM cd;

DROP VIEW IF EXISTS cd_dashboard;
CREATE VIEW cd_dashboard AS
    WITH RECURSIVE dates(date) AS (
        SELECT MIN(date) AS date FROM event
        UNION ALL
        SELECT DATE_ADD(date, INTERVAL 1 DAY)
        FROM dates
        WHERE date < (SELECT MAX(date) FROM event)
    )
    SELECT
        dates.date,
        cd.cd_number,
        cd.copies,
        (
            SELECT COUNT(DISTINCT event.id)
            FROM event
            JOIN contains ON event.playlist = contains.playlist
            WHERE contains.cd_number = cd.cd_number AND event.date = dates.date
        ) AS copies_used
    FROM dates
    CROSS JOIN cd;
