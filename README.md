# Overview

getttfb is a CLI Tool to determine the Time To First Byte for a given URL and
IP/hostname.  By default it will also attempt to upload the results to a local
Redis instance for tracking.


# Examples

This example will send a request to localhost with the host header of
"www.google.com". The "type" of check is "proxycheck" and is used in storing
the data in Redis if used. Because I'm a bit lazy in this codebase, you still
need to send all the arguments.

  getttfb www.google.com localhost proxycheck


# Redis

If configured with with Redis, the results will be stored in the Redis host
identified by the environment variables `GETTTFB_REDISHOST`, using
`GETTTFB_REDISAUTH` for the redis password on connect.

## Redis Keys

When storing result in Redis, three sorted sets are created per target and
test-type pair. The key pattern pattern is `<sitename>:<testtype>:<metric>`.

Thus if the above example is stored in Redis we would have the following
keys:

```
www.google.com:proxycheck:connect
www.google.com:proxycheck:sttfb
www.google.com:proxycheck:total
```

These represent the time to make the connection, the time the server
took to begin responding, and the total TTFB for the entire process. By
separating connection time and server response time you can determine
whether a given poor TTFB is network/client oriented (such as slow DNS
queries) or server-side.

As these are sorted sets with the timestamp as score, you can chart
these over time. If you are running multiple instances of this, say on
different hosts, to do concurrent testing I recommend embedding per-host
or per-invocation identification into the test type. 

For example if running on 5 hosts, each named `ttfb-puller-01`, you
could change `proxyconnect` to `proxyconnect|<hostname>`. This would
allow your aggregating code to identify and chart each host
individually and provide deeper metrics such as min, max, and
distribution information.


# Output

XTIMESTAMP TOTALTTFB CONNECTIONTIME SERVERTTFB

The output is tab delimited. Total TTFB is the entire process including DNS and
connection time. Connection time is just the time it takes to make a
connection, and server TTFB is the time post-connection - the actual processing
time.


# TODO

Lots of things.

 * More (some?) error checking on the Redis commands
 * Make the "test type/name" variable optional - perhaps an ENV variable?
 * Travis-ci.org integration and github release upload?
 * Create a Docker file
 * Docker Hub image auto-upload?
 * Add configurable Redis Port selection instead of assuming 6379
 * Add index for domains tested as well as for test types tested (sadd
   for each)
