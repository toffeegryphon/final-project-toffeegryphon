# Epidemic

## Brief

### Proposal
A simulation where the player acts as the government 
and tries to eradicate a disease outbreak. 
The player will have to determine if an individual is infected,
how the disease has spread, and whether (and who) to quarantine.
The player will have to balance budgetary and hospital vacancy 
constraints. Individuals will move around randomly, and have 
random chances of getting infected, and of infecting others, 
recovering, and dying. The winning condition can be eradicating
the disease in the shortest period, the least deaths, or lasting
the longest before the population dies. The player will be able
to choose a difficulty level for each feature, for example how 
asymptomatic it is, and other rates.

### Rationale
Increase understanding of epidemics, and to encourage people to
comply with disease control measures.

## Technical

### Background Knowledge
Visualization will be similar to the _Ideal Gas_ project.
Beyond that, it will involve implementing configuration states,
controls, and the simulation itself.

### Timeline

#### Week 1
1. Implement simulation model. Individual and their member
attributes (e.g. chances, rates, starting values, position), 
and interactions (e.g. movement, spreading, recovery, death).
2. Implement basic visualization. Individuals colored based on
their state. Display individuals as circles of color at their 
position.

#### Week 2
1. Implement Controls: Isolation and Emergency wards, dragging and
dropping individuals, and the simulation behind wards (e.g. 
increasing recovery rate, decrease death rate)
2. Implement Economy: Decide between actual economy with changes in
actual monetary values, or just a weekly budget and reserves.
3. Implement Power Ups: e.g. masks, lockdown, testing.

#### Week 3
1. General improvement and cleaning up, removal of half-baked
features.
2. Work on settings and difficulty selection.

#### Stretch Goals
1. Add other causes of sickness to occupy beds
2. Save and Resume
3. Leaderboards
4. Audio
5. Fetch rates and chances from online sources
6. Better animations
7. Cheat codes

### Specifics

#### Individual
- `bool is_infected_` Will be set to `true` permanently
- `bool has_recovered_` Will be set to `true` permanently
- `bool is_dead_` Will be set to `true` permanently
- `enum Status status_` `unkown, symptomatic, recovered, dying, dead`
possibly can take over role of flags above
- `bool is_sneezing_` `true` if sneezing this tick
- `vec2 spread_ (chance, roc)`
- `vec2 recovery_ (chance, roc)` Recovery + Death < 1
- `vec2 death_ (chance, roc)` Recovery + Death < 1
- `float healthiness_` Determine `recovery_` and `death_` `roc`
- (optional) `float wanderlust_` Determine chance to move far from 
original position
- `vec2 position_`
- (optional) `float compliance_` Determine effectiveness of 
power ups
- (optional) `inventory` Allows fine grain power ups (e.g. masks
per individual, testing schedule)
- (optional) `power` Determine sneeze radius

`update` Update position, perform checks and update chances:
- sneeze check and update spread chance
- recovery and death check and update statuses and chances
- decay test (if global, check test value), if 0 show result

#### City

#### Ward
- `capacity`
- `type`
- `size`
- `location`
- `struct` containing pointers to individuals, duration in ward

`update` Ticks duration, if past certain day reveal status, 
apply additional rates of change.

#### Engine
- `num_individuals`
- `num_isolation`
- `num_emergency`
- `wards`
- `individuals`

`update` Updates individuals and wards, checks for spreads
and apply them.

#### Controls
- Play/Pause and allow actions
- Click player to pick up, release to drop
- (optional) select area
- (optional) ctrl-click to select multiple

### Graphics
Deaths, City, Isolation, Emergency  
`__ __________ ____ ____`  
`|| |        | |__| |__|`  
`|| |        | |__| |__|`  
`__ |________| |__| |__|`  
