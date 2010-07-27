Client Sample -session
-------------------

This is a sample to help test http cookie session.
This sample works with session service. When the service is accessed for the
first time it will store some key value pairs in a session database. Then it will send
back the session id and expire time in a cookie header. The client send these
back in its subsequent requests to the service. Then the service will be able
to retrieve the session values corresponding to the session id from it's
internal session database.
