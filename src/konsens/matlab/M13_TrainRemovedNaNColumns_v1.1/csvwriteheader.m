function csvwriteheader( filename, header )
    [hrow hcol] = size (header);
    outid = fopen (filename, 'w+');

    % write header
    for idx = 1:hcol
        fprintf (outid, '%s', header{idx});
        if idx ~= hcol
            fprintf (outid, ',');
        else
            fprintf (outid, '\n' );
        end
    end
    % close file
    fclose(outid);
end