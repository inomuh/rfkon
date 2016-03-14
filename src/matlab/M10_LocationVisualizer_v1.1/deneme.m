if exist( 'ClassificationResults2.txt' )==0
    [hrow hcol] = size (header);
    % write header
    for idx = 1:hcol
        fprintf (outid, '%s', header{idx});
        if idx ~= hcol
            fprintf (outid, ',');
        else
            fprintf (outid, '\n' );
        end
    end
end