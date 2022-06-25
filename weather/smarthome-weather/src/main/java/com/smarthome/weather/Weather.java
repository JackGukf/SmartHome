package com.smarthome.weather;

import com.smarthome.network.Network;
import com.smarthome.dsclient.Dsclient;

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

public class Weather {

    Weather()
    {
        System.out.println( "Periodical weather forecast update!");
    }

    public static void main(String[] args)
    {
        JSONObject weatherData;
        int timeZone;
        JSONObject jo;
        BigDecimal  temp;
        int  temp2;
        Map weatherMain;
        Network network = new Network();
        try {
            weatherData = network.doQuery();
            System.out.println(weatherData);
            timeZone = (int)weatherData.get("timezone");
            System.out.println( "time zone:" + timeZone );

            // in default, java will return BigDecimal or integer,
            // can force return wanted data type value
            jo = (JSONObject)weatherData.get("main");
            temp  = (BigDecimal)jo.get("temp");
            System.out.println( "temperature:" + temp.doubleValue() );
            temp2  = (int)jo.get("humidity");
            System.out.println( "humidity:" + temp2 );

            // in default, get() will return JSON object, here to force return java map data
            weatherMain = ((JSONObject) weatherData.get("main")).toMap();
            System.out.println( weatherMain );
            // iterating address Map
            Iterator<Map.Entry> itr1 = weatherMain.entrySet().iterator();
            while (itr1.hasNext()) {
                Map.Entry pair = itr1.next();
                System.out.println(pair.getKey() + " : " + pair.getValue());
            }

            System.out.println("Connecting to Redis server" );

            Dsclient dsclient = new Dsclient();

            System.out.println("Set weather.temperature with value:" + temp);
            dsclient.jedis.set("weather.temperature", temp.toString());

            System.out.println( dsclient.jedis.get("weather.temperature") );
        }
        catch(IOException e)
        {
            System.out.println(e);
        }


    }

}
