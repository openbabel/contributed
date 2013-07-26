package com.syngenta.ciu.s4;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Executes Babel Similarity Searches.
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
public class BabelFastIndexSearch
{
    // Logger.
    private static final Logger LOG = Logger.getLogger(BabelFastIndexSearch.class.getName());

    // Temporary file affixes.
    private static final String QUERY_FILE_PREFIX = "s4_query";
    private static final String HITS_FILE_PREFIX = "s4_hits";
    private static final String TEMP_FILE_SUFFIX = ".smi";

    // Regular expression for matching Tanimoto output.
    private static final Pattern TANIMOTO_REGEX = Pattern.compile(">(.*)Tanimoto from first mol =\\s+([0-9.]+.*)");

    // Regular expression for splitting on whitespace.
    private static final Pattern WHITESPACE_REGEX = Pattern.compile("\\s+");

    // Process interrupted.
    private static final String INTERRUPTED = "Interrupted";

    // Path to index file.
    private final String indexFilePath;

    // Path to executable.
    private final String executablePathName;

    /**
     * Create the search object.
     *
     * @param babelPath path to the OpenBabel executable.
     * @param indexPath path to the index file to search.
     * @throws IOException if there are i/o problems.
     */
    public BabelFastIndexSearch(final String babelPath,
                                final String indexPath) throws IOException
    {
        // Initialize.
        indexFilePath = getIndexFilePath(indexPath);
        executablePathName = getBabelExecutablePath(babelPath);

        LOG.fine("Index file to search: " + indexFilePath);
        LOG.fine("Babel executable: " + executablePathName);
    }

    /**
     * Gets the index file path for a given value.
     *
     * @param path the value to parse and check.
     * @return the canonical form of the path to the index file.
     * @throws IOException if the path is not to a file is not readable.
     */
    private static String getIndexFilePath(final String path) throws IOException
    {
        final File file = new File(path);
        if (!file.isFile() || !file.canRead())
        {
            throw new IOException("Index file not readable file (" + path + ')');
        }

        return file.getCanonicalPath();
    }

    /**
     * Gets the Babel executable path for a given value.
     *
     * @param path the value to parse and check.
     * @return the canonical form of the path to the index file.
     * @throws IOException if the path is not to an executable file.
     */
    private static String getBabelExecutablePath(final String path) throws IOException
    {
        final File file = new File(path);
        if (!file.isFile() || !file.canExecute())
        {
            throw new IOException("Babel program not an executable file (" + path + ')');
        }

        return file.getCanonicalPath();
    }

    /**
     * Do a similarity search.
     *
     * @param smiles query structure expressed in SMILES format.
     * @param cutOff Tanimoto coefficient cut-off between 0 and 1.
     * @return The search hits.
     * @throws IOException if there are i/o problems.
     */
    public String doSimilaritySearch(final String smiles,
                                     final double cutOff) throws IOException
    {
        LOG.fine("Similarity search of " + indexFilePath + " for " + smiles + " (cut-off=" + cutOff + ')');

        // Write SMILES to a query file.
        final File queryFile = writeQueryFile(smiles);

        // Create temporary file for results.
        final File resultsFile = File.createTempFile(HITS_FILE_PREFIX, TEMP_FILE_SUFFIX);

        // Storage for results.
        final StringBuilder results = new StringBuilder(100);

        // Search against the index file.
        similaritySearch(cutOff, queryFile, resultsFile);
        if (resultsFile.length() > 0L)
        {
            // Generate Tanimoto coefficients for each hit.
            final Map<String, Double> map = tanimoto(queryFile, resultsFile);

            // Read the results file.
            final BufferedReader reader = new BufferedReader(new FileReader(resultsFile));
            try
            {
                String line;
                while ((line = reader.readLine()) != null)
                {
                    final String[] strings = WHITESPACE_REGEX.split(line, 2);
                    if (strings.length == 2)
                    {
                        // Get SMILES, ID and Tanimoto coefficient.
                        final String structure = strings[0].trim();
                        String id = strings[1].trim();
                        final Double tanimoto = map.get(id);
                        if (tanimoto == null)
                        {
                            LOG.warning("Could not find Tanimoto coefficient for structure (" + id + ')');
                        }
                        else
                        {
                            // Should only be queried once.
                            map.remove(id);

                            // Replace separator.
                            id = id.replace('\t', ' ');

                            // Append record.
                            results.append(id + '\t' + structure + '\t' + tanimoto + '\n');
                        }
                    }
                    else
                    {
                        LOG.warning("Ignored invalid line of hits file: " + line);
                    }
                }

                if (!map.isEmpty())
                {
                    LOG.warning("Results file did not contain the same number of structures as the Tanimoto map.");
                }
            }
            finally
            {
                reader.close();
            }
        }

        // 4. Merge results.

        // Clean up.
        LOG.fine("Cleaning up...");
        queryFile.delete();
        resultsFile.delete();

        return results.toString();
    }

    /**
     * Do a substructure search.
     *
     * @param smarts query expressed in SMARTS format.
     * @return The search hits.
     * @throws IOException if there are i/o problems.
     */
    public String doSubstructureSearch(final String smarts) throws IOException
    {
        LOG.fine("Sub-structure search of " + indexFilePath + " for " + smarts);

        // Create temporary file for results.
        final File resultsFile = File.createTempFile(HITS_FILE_PREFIX, TEMP_FILE_SUFFIX);

        // Storage for results.
        final StringBuilder results = new StringBuilder(100);

        // Search against the index file.
        substructureSearch(resultsFile,
                           smarts);
        if (resultsFile.length() > 0L)
        {
            // Read the results file.
            final BufferedReader reader = new BufferedReader(new FileReader(resultsFile));
            try
            {
                String line;
                while ((line = reader.readLine()) != null)
                {
                    // Append record.
                    results.append(line);
                }
            }
            finally
            {
                reader.close();
            }
        }

        // Clean up.
        LOG.fine("Cleaning up...");
        resultsFile.delete();

        return results.toString();
    }

    /**
     * Write the SMILES query to a file.
     *
     * @param smiles the SMILES to query.
     * @return a handle for the query file.
     * @throws IOException if there are i/o problems.
     */
    private static File writeQueryFile(final String smiles) throws IOException
    {
        final File queryFile = File.createTempFile(QUERY_FILE_PREFIX, TEMP_FILE_SUFFIX);
        final FileWriter writer = new FileWriter(queryFile);
        try
        {
            writer.append(smiles + '\n');
        }
        finally
        {
            writer.close();
            LOG.fine("Wrote query file: " + queryFile);
        }
        return queryFile;
    }

    /**
     * Execute the similarity search.
     *
     * @param cutOff      the Tanimoto coefficient threshold.
     * @param queryFile   query SMILES file.
     * @param resultsFile file into which hits are written.
     * @throws IOException if there are i/o problems.
     */
    private void similaritySearch(final double cutOff,
                                  final File queryFile,
                                  final File resultsFile) throws IOException
    {
        // Create the process.
        final Process process = new ProcessBuilder(executablePathName,
                                                   indexFilePath,
                                                   resultsFile.getCanonicalPath(),
                                                   "-S" + queryFile.getCanonicalPath(),
                                                   "-at" + cutOff).start();

        // Log STDERR..
        final StringBuilder err = new StringBuilder(100);
        final BufferedReader stdErr = new BufferedReader(new InputStreamReader(process.getErrorStream()));
        try
        {
            String line;
            while ((line = stdErr.readLine()) != null)
            {
                err.append(line);
                LOG.info(line);
            }
        }
        finally
        {
            stdErr.close();
        }

        // Wait for exit.
        try
        {
            process.waitFor();
        }
        catch (InterruptedException e)
        {
            LOG.warning(INTERRUPTED);
        }

        // Check exit value.
        if (process.exitValue() != 0)
        {
            throw new IllegalStateException("Babel search (similarity) failed.\nError messages:\n" + err);
        }
    }

    /**
     * Execute the substructure search.
     *
     * @param resultsFile file into which hits are written.
     * @param smarts      substructure query.
     * @throws IOException if there are i/o problems.
     */
    private void substructureSearch(final File resultsFile,
                                    final String smarts) throws IOException
    {
        // Create the process.
        final Process process = new ProcessBuilder(executablePathName,
                                                   indexFilePath,
                                                   "-ifs",
                                                   resultsFile.getCanonicalPath(),
                                                   "-s" + smarts).start();

        // Log STDERR..
        final StringBuilder err = new StringBuilder(100);
        final BufferedReader stdErr = new BufferedReader(new InputStreamReader(process.getErrorStream()));
        try
        {
            String line;
            while ((line = stdErr.readLine()) != null)
            {
                err.append(line);
                LOG.info(line);
            }
        }
        finally
        {
            stdErr.close();
        }

        // Wait for exit.
        try
        {
            process.waitFor();
        }
        catch (InterruptedException e)
        {
            LOG.warning(INTERRUPTED);
        }

        // Check exit value.
        if (process.exitValue() != 0)
        {
            throw new IllegalStateException("Babel search (sub-structure) failed.\nError messages:\n" + err);
        }
    }

    /**
     * Calculate Tanimoto coefficients between query structure and hits.
     *
     * @param queryFile   SMILES file holding query structure.
     * @param resultsFile SMILES file holding hits.
     * @return a map of structure IDs from the results file to Tanimoto coefficients for the hits structures vs. the query structure.
     * @throws IOException if there are i/o problems.
     */
    private Map<String, Double> tanimoto(final File queryFile,
                                         final File resultsFile) throws IOException
    {
        // Create a map to hold IDs -> Tanimoto coefficient.
        final Map<String, Double> map = new HashMap<String, Double>(10);


        // Execute the OpenBabel process: babel query.smi hits.smi -ofpt
        final Process process = new ProcessBuilder(executablePathName,
                                                   queryFile.getCanonicalPath(),
                                                   resultsFile.getCanonicalPath(),
                                                   "-ofpt").start();

        // Capture STDIN.
        final BufferedReader stdIn = new BufferedReader(new InputStreamReader(process.getInputStream()));
        try
        {
            String line;
            while ((line = stdIn.readLine()) != null)
            {
                // Match Tanimoto output lines.
                final Matcher matcher = TANIMOTO_REGEX.matcher(line);
                if (matcher.matches())
                {
                    map.put(matcher.group(1).trim(), Double.parseDouble(matcher.group(2).trim()));
                }
            }
        }
        finally
        {
            stdIn.close();
        }

        // Log STDERR..
        final StringBuilder err = new StringBuilder(100);
        final BufferedReader stdErr = new BufferedReader(new InputStreamReader(process.getErrorStream()));
        try
        {
            String line;
            while ((line = stdErr.readLine()) != null)
            {
                LOG.info(line);
                err.append(line);
            }
        }
        finally
        {
            stdErr.close();
        }

        // Wait for exit.
        try
        {
            process.waitFor();
        }
        catch (InterruptedException e)
        {
            LOG.warning(INTERRUPTED);
        }

        // Check exit value.
        if (process.exitValue() != 0)
        {
            throw new IllegalStateException("Babel search (Tanimoto) failed.\nError messages:\n" + err);
        }

        return map;
    }
}
