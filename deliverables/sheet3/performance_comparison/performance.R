library(ggplot2)

performance <- read.csv("data.csv")

ggplot(performance, aes(num_particles, time, color = implementation)) +
  geom_point(size = 2) +
  geom_line(linewidth = 1) +
  scale_color_manual(
    name="Implementation",
    labels = c("Linked Cell","Naive"),
    values = c("turquoise3","brown1")
  ) +
  labs(
    title = "Performance Comparison of Particle Container Implementations",
    x = "Number of Particles",
    y = "Time (in ms)",
  )
