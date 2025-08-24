# Options Pricing Model

## Description
- Calculate exact values of options contracts with the Black-Scholes formula.  
- Approximate values of options with a binomial tree or Monte Carlo simulation.

## Building

```zsh
mkdir build
cd build
cmake .
make options_pricer_cli
```

## Running

The below will run the Black-Scholes formula for a European call option 
1. Underlying price 100.30
2. Strike price 105.50
3. 2.4 units of time to expiry
4. A risk free rate of return of 4.5 percent per unit time
5. An underlying volatility of 0.65 (standard deviation of log returns over time horizon = 1 unit of time)

```zsh
./options_pricer_cli -u 100.30 -s 105.50 -t 2.4 -r 0.045 -v 0.65 -b 10000 -m 100 1000000
```

## Flags
`-p, --put` to calculate prices for a put instead of a call  

`-a, --american` to calculate prices for American options instead of European
