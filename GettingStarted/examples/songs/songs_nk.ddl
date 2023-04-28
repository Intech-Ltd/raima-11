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

database songs_nk inmemory {
    data    file "songs.d01" contains album;
    data    file "songs.d02" contains artist;
    data    file "songs.d03" contains genre;
    data    file "songs.d04" contains track;
    vardata file "songs.v01" contains album_name;
    vardata file "songs.v02" contains track_name;

    record album {
        varchar album_name[45];
    }

    record artist {
        char artist_name[25];
    }

    record genre {
        char genre_name[25];
    }

    record track {
        varchar track_name[45];
        char track_id[8];
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

