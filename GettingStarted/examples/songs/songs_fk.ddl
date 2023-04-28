/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

database songs_fk inmemory {
    data    file "songs.d01" contains album;
    data    file "songs.d02" contains artist;
    data    file "songs.d03" contains genre;
    data    file "songs.d04" contains track;
    vardata file "songs.v01" contains album_name;
    vardata file "songs.v02" contains track_name;
    key     file "songs.k01" contains album_name;
    key     file "songs.k02" contains artist_name;
    key     file "songs.k03" contains genre_name;
    key     file "songs.k04" contains track_name;
    key     file "songs.k05" contains track_id;

    record album {
        key varchar album_name[45];
    }

    record artist {
        key char artist_name[25];
    }

    record genre {
        key char genre_name[25];
    }

    record track {
        key varchar track_name[45];
        unique key char track_id[8];
        int16_t track_year;
    }

    set album_tracks {
        order last;
        owner album;
        member track;
    }

    set artist_tracks {
        order last;
        owner artist;
        member track;
    }

    set genre_tracks {
        order last;
        owner genre;
        member track;
    }

    set artist_albums {
        order last;
        owner artist;
        member album;
    }
}

