package com.syngenta.ciu.s4;

import org.simpleframework.http.Query;
import org.simpleframework.http.Request;
import org.simpleframework.http.Response;
import org.simpleframework.http.Status;
import org.simpleframework.http.core.Container;
import org.simpleframework.http.core.ContainerServer;
import org.simpleframework.transport.connect.SocketConnection;

import java.io.*;
import java.net.InetSocketAddress;
import java.util.logging.Logger;

/**
 * Launches S4 (Simple Structure Search Server).
 *
 * @author $Author$
 * @version $Revision$
 *          <p/>
 *          This file is part of the Open Babel project. For more information, see <http://openbabel.sourceforge.net/> This program is free software;
 *          you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation
 *          version 2 of the License. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 *          implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *          <p/>
 *          Code created was by or on behalf of Syngenta and is released under the open source license in use for the pre-existing code or project.
 *          Syngenta does not assert ownership or copyright any over pre-existing work.
 */
public class Server
        implements Container
{
    // Logger.
    private static final Logger LOG = Logger.getLogger(Server.class.getName());

    // The server name.
    private static final String SERVER_NAME = "Simple Similarity Search Server/1.0 (Simple 5.0.7)";

    // Property names.
    private static final String PORT_PROPERTY_NAME = "s4.server.port.number";
    private static final String INDEX_FILE_PROPERTY_NAME = "s4.index.file.name";
    private static final String BABEL_PATH_PROPERTY_NAME = "s4.babel.exe.path.name";

    // Default port number.
    private static final int DEFAULT_PORT_NUMBER = 8080;

    // Search URI paths.
    private static final String SIMILARITY_SEARCH_PATH = "simsearch";
    private static final String SUBSTRUCTURE_SEARCH_PATH = "subsearch";

    // Default similarity search cut-off.
    private static final double DEFAULT_SIMILARITY_CUT_OFF = 0.8;

    // Query parameters.
    private static final String SMILES_QUERY_PARAMETER = "smiles";
    private static final String SMARTS_QUERY_PARAMETER = "smarts";
    private static final String CUT_OFF_QUERY_PARAMETER = "cutoff";

    // Babel search object.
    private final BabelFastIndexSearch babel;

    /**
     * Create the container.
     *
     * @param search the structure search object.
     */
    private Server(final BabelFastIndexSearch search)
    {
        babel = search;
    }

    /**
     * Handle a request.
     *
     * @param req  the request that contains the client HTTP message
     * @param resp the response used to deliver the server response
     */
    public void handle(final Request req,
                       final Response resp)
    {
        try
        {
            LOG.fine("Handling request: " + req.getTarget());

            final String content;
            final String path = req.getPath().getPath();
            if (path != null && path.equalsIgnoreCase('/' + SIMILARITY_SEARCH_PATH))
            {
                // Similarity search.
                String text;
                try
                {
                    final Query query = req.getQuery();
                    text = babel.doSimilaritySearch(getSearchQuery(query, SMILES_QUERY_PARAMETER), getCutOff(query));
                }
                catch (Exception ex)
                {
                    text = badRequest(resp, ex);
                }

                content = text;
            }
            else if (path != null && path.equalsIgnoreCase('/' + SUBSTRUCTURE_SEARCH_PATH))
            {
                // Substructure search.
                String text;
                try
                {
                    final Query query = req.getQuery();
                    text = babel.doSubstructureSearch(getSearchQuery(query, SMARTS_QUERY_PARAMETER));
                }
                catch (Exception ex)
                {
                    text = badRequest(resp, ex);
                }

                content = text;
            }
            else
            {
                // Request for an invalid path.
                content = "The requested page requested does not exist.";
                resp.setCode(Status.NOT_FOUND.code);
                resp.setDescription(Status.NOT_FOUND.description);
            }

            // Set response header.
            final long time = System.currentTimeMillis();
            resp.setValue("Server", SERVER_NAME);
            resp.setDate("Date", time);
            resp.setDate("Last-Modified", time);
            resp.setValue("Content-Type", "text/plain");

            // Set body content.
            final PrintStream body = resp.getPrintStream();
            body.println(content);
            body.close();

            LOG.fine("Response " + resp.getCode() + ' ' + resp.getDescription());
        }
        catch (Exception e)
        {
            LOG.warning("Failed to handle request: " + e.getMessage());
        }
    }

    /**
     * Handle bad requests.
     *
     * @param resp response.
     * @param ex   exception.
     * @return text to add to response.
     */
    private String badRequest(Response resp,
                              Exception ex)
    {
        resp.setCode(Status.BAD_REQUEST.code);
        resp.setDescription(Status.BAD_REQUEST.description);
        return "The request was not understood.\n" + ex.getMessage();
    }

    /**
     * Gets the cut-off value from the request query.
     *
     * @param query the request query.
     * @return the value of the cut-off parameter.
     */
    private static double getCutOff(final Query query)
    {
        final String val = query.get(CUT_OFF_QUERY_PARAMETER);
        final double cutOff =
                val == null || val.trim().length() == 0 ? DEFAULT_SIMILARITY_CUT_OFF : Double.parseDouble(val.trim());
        if (cutOff < 0.0 || cutOff > 1.0)
        {
            throw new IllegalArgumentException("Cut-off must be between 0 and 1");
        }
        return cutOff;
    }

    /**
     * Gets the search query parameter value from the request query.
     *
     * @param query     the request query.
     * @param parameter the search query.
     * @return the value of the search query parameter.
     */
    private static String getSearchQuery(final Query query,
                                         final String parameter)
    {
        String search = query.get(parameter);
        if (search == null || search.trim().length() == 0)
        {
            throw new IllegalArgumentException(parameter + " query parameter not specified");
        }
        search = search.trim();
        return search;
    }

    /**
     * Gets a compulsory system property value.
     *
     * @param name the property name.
     * @return the value of the property.
     * @throws IllegalArgumentException if the property is not set.
     */
    private static String getPropertyNotNull(final String name)
    {
        final String pathName = System.getProperty(name);
        if (pathName == null)
        {
            throw new IllegalArgumentException("No value specified for " + name);
        }
        return pathName;
    }

    /**
     * Application entry point.
     *
     * @param list command-line arguments.
     * @throws IOException if there are i/o problems.
     */
    public static void main(final String... list) throws IOException
    {
        // Create a Babel search object.
        final BabelFastIndexSearch search = new BabelFastIndexSearch(getPropertyNotNull(BABEL_PATH_PROPERTY_NAME),
                                                                     getPropertyNotNull(INDEX_FILE_PROPERTY_NAME));

        // Get port number.
        final int port =
                Integer.parseInt(System.getProperty(PORT_PROPERTY_NAME, Integer.toString(DEFAULT_PORT_NUMBER)));

        // Start server.
        new SocketConnection(new ContainerServer(new Server(search))).connect(new InetSocketAddress(port));
        LOG.info("Server started, listening on port " + port + "...");
    }
}