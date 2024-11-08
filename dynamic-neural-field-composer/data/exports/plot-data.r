# Load necessary libraries
library(ggplot2)
library(extrafont)  # For additional fonts

# Import fonts (only necessary if the font isn't yet registered in R)
# font_import(pattern = "Century") # Uncomment if you need to import the font
loadfonts(device = "win")  # Adjust for your OS; e.g., use "mac" for macOS

# Load data from the text files
u_input <- scan("./neural field u_input_2024-11-06_10-57-01.4994485.txt", what = numeric())
u_activation <- scan("./neural field u_activation_2024-11-06_10-56-59.9490110.txt", what = numeric())

# Combine data into a data frame for plotting
data <- data.frame(
  Index = 1:length(u_input),
  u_input = u_input,
  u_activation = u_activation
)

# Plotting with a simple, modern design
ggplot(data, aes(x = Index)) +
  geom_line(aes(y = u_input), color = "#4C6EF5", size = 2) +
  geom_line(aes(y = u_activation), color = "#E35D6A", size = 2) +
  labs(
    title = "Neural Field Input vs Activation",
    x = "Index",
    y = "Value"
  ) +
  theme_minimal(base_family = "Century Schoolbook") +
  theme(
    plot.title = element_text(hjust = 0.5, size = 16, face = "bold"),
    axis.title = element_text(size = 12),
    axis.text = element_text(size = 10),
    legend.position = "none",
    plot.background = element_rect(fill = "white", color = NA)
  )
