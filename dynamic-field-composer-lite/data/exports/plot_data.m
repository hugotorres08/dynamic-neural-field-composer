% Load Data from TXT File
data = dlmread('NeuralField_activation_2023-12-21_10-10-16.8630546.txt');

% Customize Plot
figure;

% Plotting the data as an area with solid line and semi-transparent fill
% fill_area = fill([1:length(data), fliplr(1:length(data))], [data, zeros(size(data))], [0.2, 0.4, 0.8], 'EdgeColor', 'none', 'FaceAlpha', 0.3);
hold on;
plot(data, 'LineWidth', 3, 'Color', [0.2, 0.4, 0.8]);

% Add labels and title
xlabel('X-axis Label');
ylabel('Y-axis Label');
title('Plot');

% Customize axes limits
xlim([1, length(data)]);
ylim([min(data), max(data)]);

% Add grid and minor ticks
grid on;
set(gca, 'XMinorGrid', 'on', 'YMinorGrid', 'on');

% Add legend
legend('Data', 'Location', 'Best');

% Customize font and size
set(gca, 'FontName', 'AvantGarde', 'FontSize', 10);

% Adjust axes properties
set(gca, 'Box', 'off', 'TickDir', 'out', 'TickLength', [0.02, 0.02], ...
    'XMinorTick', 'on', 'YMinorTick', 'on', 'YGrid', 'on', ...
    'XColor', [0.3, 0.3, 0.3], 'YColor', [0.3, 0.3, 0.3], 'LineWidth', 1);

% Set figure aspect ratio correctly
pbaspect([2, 1, 1]);  % [width, height, depth]

% Remove excess padding
set(gca, 'LooseInset', get(gca, 'TightInset'));


% Save the figure
%print('plot', '-dpdf', '-r300');
