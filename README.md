# Monte Carlo Simulation

This is a simulation to find the probability of running out of money given a set of investments. It takes ideas and data from chapter 12 from [Impractical Python Projects](https://nostarch.com/impracticalpythonprojects), though the code is *completely* written from scratch. 

## Monte Carlo Explanation

There are many factors and details that go into how well one will do economically. As the number of factors is very large, it is very infeasible to keep track of each of them. 

Thus, it is easier to keep track of a select few important factors, and use random chance simulations to find the **probability** of a desired outcome given those factors. While it will not exactly tell what will happen because of the lack of factors, a Monte Carlo simulation, a simulation that uses random chance to compute a probability, can help make decisions.

The Monte Carlo simulation can account for both good and bad times, anticipating the unexpected through historical data. Many lives are simulated as using the same factors as the user in various historical periods, records for which provide much of the simulation's biases in its random number generation.

## Pseudocode

- Get the various factors the user has inputted:
    - investment blend (money invested in stocks to money invested in bonds)
    - initial investment value
    - yearly expenses
    - duration of retirement (minimum, maximum, most probable)
- Find historical data on the returns of each investment selected. 
- Begin a list of outcomes (vector<tuple<double, double, double>> storing start years, end years, and ending investment values)
- Repeat CASE_AMOUNT times:
    - Randomly generate an interval (recorded in the historical data). 
        - This interval's length is based on a triangular distribution (min, probable, max)
        - Get the returns for the investments and inflation rates in this period.
    - For each year in this interval's length:
        - Find the amount expended (adjusted for inflation).
        - Adjust investments by subtracting expenses and adding returns (from historical records).
    - Add the investments' final value to the outcomes list. 
- Find the amount of investments' whose final values are less than 0.
- Use this to print the probability of ruin.
- Find the best cases in the outcomes and print out which years they were in. 
- Plot the various final values in a graph. 