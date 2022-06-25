package com.smarthome.dsclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.util.Map;
import java.util.Iterator;
import java.math.BigDecimal;

import org.json.JSONException;
import org.json.JSONObject;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPooled;

public class Dsclient extends JedisPooled {

    String dsServer;
    int dsServerPort;
    public JedisPooled jedis;

    public Dsclient()
    {
        this.dsServer = "localhost";
        this.dsServerPort = 6379;
        System.out.println( "Connecting to data store server:" + this.dsServer + " on port:" + this.dsServerPort );
        this.jedis = new JedisPooled(this.dsServer, this.dsServerPort);
    }

    public Dsclient( String server, int port )
    {
        this.dsServer = server;
        this.dsServerPort = port;
        System.out.println( "Connecting to data store server:" + this.dsServer + " on port:" + this.dsServerPort );
        this.jedis = new JedisPooled(this.dsServer, this.dsServerPort);
    }
}
