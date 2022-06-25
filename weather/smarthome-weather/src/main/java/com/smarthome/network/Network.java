package com.smarthome.network;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.util.Map;
import java.util.Iterator;
import java.math.BigDecimal;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;

import org.json.JSONException;
import org.json.JSONObject;


public class Network {

    String city;
    String country ;
    final static String web = "https://api.openweathermap.org/data/2.5/weather?";
    String url;
    private String privateKey = "051f78ce6f6ce5cf7124ea10d02bf531";

    private HttpClient httpClient;

    public Network()
    {
        this.city = "Vancouver";
        this.country = "ca";
        this.url = web + "q=" + city + ',' + country + "&units=metric" + "&APPID=" + privateKey;
        this.httpClient = HttpClientBuilder.create().build();

        System.out.println(this.url);
    }
    public Network( String city, String country )
    {
        this.city = city;
        this.country = country;
        this.url = web + "q=" + city + ',' + country + "&units=metric" + "&APPID=" + privateKey;
        this.httpClient = HttpClientBuilder.create().build();

        System.out.println(this.url);
    }

    Network (HttpClient httpClient) {
        if (httpClient == null)
            throw new IllegalArgumentException ("Can't construct a OwmClient with a null HttpClient");
        this.city = "Vancouver";
        this.country = "ca";
        this.url = web + "q=" + city + ',' + country + "&units=metric" + "&APPID=" + privateKey;
        this.httpClient = httpClient;
    }

    Network( JSONObject config )
    {
        // to be implemented with json config file

    }

    public JSONObject doQuery() throws JSONException, IOException {
        String responseBody = null;

        HttpGet httpget = new HttpGet (this.url);

        HttpResponse response = this.httpClient.execute (httpget);
        InputStream contentStream = null;
        try {
                StatusLine statusLine = response.getStatusLine ();
                if (statusLine == null) {
                    throw new IOException (
                        String.format ("Unable to get a response from OWM server"));
            }
            int statusCode = statusLine.getStatusCode ();
            if (statusCode < 200 && statusCode >= 300) {
                throw new IOException (
                        String.format ("OWM server responded with status code %d: %s", statusCode, statusLine));
            }
            /* Read the response content */
            HttpEntity responseEntity = response.getEntity ();
            contentStream = responseEntity.getContent ();
            Reader isReader = new InputStreamReader (contentStream);
            int contentSize = (int) responseEntity.getContentLength ();
            if (contentSize < 0)
                contentSize = 8*1024;
            StringWriter strWriter = new StringWriter (contentSize);
            char[] buffer = new char[8*1024];
            int n = 0;
            while ((n = isReader.read(buffer)) != -1) {
                strWriter.write(buffer, 0, n);
            }
            responseBody = strWriter.toString ();
            contentStream.close ();
        } catch (IOException e) {
            throw e;
        } catch (RuntimeException re) {
            httpget.abort ();
            throw re;
        } finally {
            if (contentStream != null)
                contentStream.close ();
        }
        return new JSONObject (responseBody);
    }
}
