function [] = plotLogFile( filename )
    axes('FontSize', 10,'fontWeight','bold');
    file = fopen(filename);
    head = textscan(file,'%s',14,'delimiter',',');
    csv = textscan(file,'%s %d %d %s %d %f %f %d %d %f %f %f %f %s','delimiter',',');
    fclose(file);
    
    Heur = csv{1}; Task = csv{2}; Proc = csv{3}; Status = csv{4};
    Index = csv{5}; Time = csv{6}; Fitness	= csv{7}; Loop = csv{8};
    Count = csv{9}; Best = csv{10}; Worst = csv{11}; Mean = csv{12};
    Std = csv{13}; Sol = csv{14}; N = length(Time);
    
    preProccess();
    cmax = numProc();
    cmap = hsv( double(cmax) ); % cmap = rand( cmax, 3);   
   
    xlabel('Time(s)'); ylabel('Fitness'); 
      
    hold on; grid on;
    for i = 0 : numTask() - 1          
        if strcmp(heurictis(i), 'HC') || strcmp(heurictis(i), 'SA') || strcmp(heurictis(i), 'TS') 
            n = count(i) - 1;  
            for j = 0 : n  
                x = time(i, j); 
                y = fitness(i, j);
                
                gra(j+1) = plot( x, y, 'Color', cmap( proc(i) + 1,:));     
                
                if strcmp(heurictis(i), 'HC')
                    set(gra(j+1), 'Marker', 's', ...
                        'MarkerEdgeColor','r', ...
                        'MarkerFaceColor', 'r' );
                end
                if strcmp(heurictis(i), 'SA')
                    set(gra(j+1), 'Marker', '^', ...
                        'MarkerEdgeColor','b', ...
                        'MarkerFaceColor', 'b' );
                end
                if strcmp(heurictis(i), 'TS')
                    set(gra(j+1), 'Marker', 'o', ...
                        'MarkerEdgeColor','g', ...
                        'MarkerFaceColor', 'g' );    
                end
                set(gra(j+1), 'MarkerSize', 3.0 );
                set(gra(j+1), 'LineWidth', 2.0 );
            end 
        
        elseif strcmp(heurictis(i), 'GA') || strcmp(heurictis(i), 'MA')
            if strcmp(heurictis(i), 'GA')
                pl(1)= plot( time(i), average(i), 'Color', cmap( proc(i) + 1,:), 'LineWidth', 4.0);
                pl(2) = errorbar( time(i), average(i), stdd(i), 'sb', 'MarkerSize', 8.0 );
                pl(3)= plot( time(i), best(i), 'r:');
                pl(4) = plot( time(i), worst(i), 'r:');
            end
            if strcmp(heurictis(i), 'MA')
                pl(1)= plot( time(i), average(i), 'Color', cmap( proc(i) + 1,:), 'LineWidth', 4.0);
                pl(2) = errorbar( time(i), average(i), stdd(i), 'sm', 'MarkerSize', 8.0 ); 
                pl(3)= plot( time(i), best(i), 'b:');
                pl(4) = plot( time(i), worst(i), 'b:');
            end    
        end  
    end
    
    % dummy graph
    dum = []; text = [];
    
    for j = 0 : numProc() - 1  
        for i = 0 : numTask() - 1  
            if j == proc(i)
                graph = [];
                if strcmp(heurictis(i), 'HC') || strcmp(heurictis(i), 'SA') || strcmp(heurictis(i), 'TS') 
                    graph = plot( time(i, 0), fitness(i, 0), 'Color', cmap( proc(i) + 1,:), 'LineWidth', 2.0);  
                elseif strcmp(heurictis(i), 'GA') || strcmp(heurictis(i), 'MA')
                    graph = plot( time(i), average(i), 'Color', cmap( proc(i) + 1,:), 'LineWidth', 4.0);  
                end
                if isempty(graph) == 0
                    dum = [dum ; graph];
                    if isempty(text)
                        text = char(strcat('Proc ', num2str(j) ) );
                    else
                        text = char(text, strcat('Proc ', num2str(j) ) );
                    end
                end
                break;
            end
        end
    end    
    for i = 0 : numTask() - 1  
        if strcmp(heurictis(i), 'HC') == 1  
            graph = plot( time(i, 0), fitness(i, 0), 'ks', 'MarkerSize', 3.0, 'MarkerEdgeColor', 'r' ,'MarkerFaceColor', 'r' );
            dum = [dum ; graph];
            text = char(text, 'HC');
            break;
        end
    end
    for i = 0 : numTask() - 1  
        if strcmp(heurictis(i), 'SA') == 1  
            graph = plot( time(i, 0), fitness(i, 0), 'k^', 'MarkerSize', 3.0, 'MarkerEdgeColor', 'b' ,'MarkerFaceColor', 'b' );
            dum = [dum ; graph];
            text = char(text, 'SA');
            break;
        end
    end
    for i = 0 : numTask() - 1  
        if strcmp(heurictis(i), 'TS') == 1  
            graph = plot( time(i, 0), fitness(i, 0), 'ko', 'MarkerSize', 3.0, 'MarkerEdgeColor', 'g' ,'MarkerFaceColor', 'g' );
            dum = [dum ; graph];
            text = char(text, 'TS');
            break;
        end
    end
    for i = 0 : numTask() - 1  
        if strcmp(heurictis(i), 'GA') == 1  
            graph = errorbar( time(i), average(i), stdd(i), 'sb', 'MarkerSize', 8.0); 
            dum = [dum ; graph];
            text = char(text, 'GA');
            break;
        end
    end
    for i = 0 : numTask() - 1  
        if strcmp(heurictis(i), 'MA') == 1  
            graph = errorbar( time(i), average(i), stdd(i),'sm', 'MarkerSize', 8.0) ;
            dum = [dum ; graph];
            text = char(text, 'MA');
            break;
        end
    end
  
    if isempty(dum) == 0
        legend(dum, text, 'Location', 'SouthEast');
    end
%    set(findall(gcf,'type','text'),'FontSize',11,'fontWeight','bold')
 
    function result = fitness(task, id)
        result = [];
        if nargin == 1
            for ii = 1 : N
                if( Task(ii) == task )
                    result = [ result; Fitness(ii) ]; 
                end
            end
        end
        if nargin == 2
            for ii = 1 : N
                if( Task(ii) == task && Index(ii) == id )
                    result = [ result; Fitness(ii) ]; 
                end
            end
        end
    end

    function result = average(task)
        result = [];
        for ii = 1 : N
            if( Task(ii) == task)
                result = [ result; Mean(ii) ]; 
            end
        end
    end

    function result = time(task, id)
        result = [];
        if nargin == 1
            for ii = 1 : N
                if( Task(ii) == task )
                    result = [ result; Time(ii) ]; 
                end
            end
        end
        if nargin == 2
            for ii = 1 : N
                if( Task(ii) == task && Index(ii) == id )
                    result = [ result; Time(ii) ]; 
                end
            end
        end
    end

    function result = best(task)
        result = [];
        for ii = 1 : N
            if( Task(ii) == task )
                result = [ result; Best(ii) ]; 
            end
        end
    end

    function result = worst(task)
        result = [];
        for ii = 1 : N
            if( Task(ii) == task )
                result = [ result; Worst(ii) ]; 
            end
        end
    end

    function result = stdd(task)
        result = [];
        for ii = 1 : N
            if( Task(ii) == task )
                result = [ result; Std(ii) ]; 
            end
        end
    end
    
    function result = proc(task, id)
        result = 0;
        if nargin == 1
            for ii = 1 : N
                if( Task(ii) == task )
                    result = Proc(ii);
                    break;
                end
            end
        end
        if nargin == 2
            for ii = 1 : N
                if( Task(ii) == task && Index(ii) == id )
                    result = Proc(ii);
                    break;
                end
            end
        end
    end

    function result = heurictis(task)
        result = [];
        for ii = 1 : N
            if( Task(ii) == task )
                result = Heur(ii);
                break;
            end
        end
        if isempty(result) == 1
            result = '';
        end
    end

    function result = numTask()
        result = 0;
        for ii = 1 : N
            if( Task(ii) > result)
                result = Task(ii); 
            end
        end
        result = result + 1;
    end

    function result = numProc()
        result = 0;
        for ii = 1 : N
            if( Proc(ii) > result)
                result = Proc(ii); 
            end
        end
        result = result + 1;
    end

    function result = loopInTask(task) 
        result = 0;
        for ii = 1 : N
            if( Task(ii) == task )
                if strcmp(Heur(ii), 'MA') || strcmp(Heur(ii), 'GA')
                    if strcmp(Status(ii), 'S') == 1 
                        result = result + 1;
                    end
                elseif strcmp(Heur(ii), 'HC') || strcmp(Heur(ii), 'SA') || strcmp(Heur(ii), 'TS') 
                    if strcmp(Status(ii), 'S') == 1 && Index(ii) == 0
                        result = result + 1;
                    end
                end
            end
        end
    end

    function result = count(task) 
        for ii = 1 : N
            if( Task(ii) == task )
                result = Count(ii);
                break;
            end
        end
    end
    
    function preProccess()
        ntask__ = numTask();  
        plus__ = 0;
        for task__ = 0 : ntask__ - 1            
            loop__ = loopInTask(task__);
            if loop__ > 1
                index__ = -1;  
                for ii = 1 : N
                    if Task(ii) == task__
                        if strcmp(Heur(ii), 'MA') || strcmp(Heur(ii), 'GA')
                            if strcmp(Status(ii), 'S') == 1
                                index__ = index__ + 1;
                            end
                            if index__ > 0
                                Task(ii) = ntask__ + index__ + plus__ - 1;
                            end
                        elseif  strcmp(Heur(ii), 'HC') || strcmp(Heur(ii), 'SA') || strcmp(Heur(ii), 'TS') 
                            if strcmp(Status(ii), 'S') == 1 && Index(ii) == 0
                                index__ = index__ + 1;
                            end
                            if index__ > 0
                                Task(ii) = ntask__ + index__ + plus__ - 1;
                            end
                        end
                    end                   
                end
                plus__ = plus__ + index__;
            end
        end
    end
end




