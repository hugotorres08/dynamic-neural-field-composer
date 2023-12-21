% Load the data
data = load('NeuralField_activation_2023-12-20_14-19-29.2930895.txt');

% Create a spatial vector (assuming a spatial step of 1 )
space = 0:1:length(data)-1;

% Plot the data
plot(space, data);

% Add labels and title
xlabel('Space');
ylabel('Value');
title('Example Plot');

% Show the grid
grid on;

% Show the plot
figure;