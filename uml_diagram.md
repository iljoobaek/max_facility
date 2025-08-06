# UML Class Diagram - Central Fill Facility System

```
┌─────────────────────────────────────┐
│           CentralFillFacility       │
├─────────────────────────────────────┤
│ - id: int                           │
│ - x: int                            │
│ - y: int                            │
│ - dist: int                         │
│ - medications: vector<string>       │
│ - prices: vector<float>             │
├─────────────────────────────────────┤
│ + CentralFillFacility(int, int,     │
│   int, vector<string>&,             │
│   vector<float>&)                   │
│ + getId(): int                      │
│ + getCoordinates(): pair<int,int>   │
│ + getMedications(): vector<string>  │
│ + getPrices(): vector<float>        │
│ + getDistance(): int                │
│ + setDistance(int): void            │
│ + operator<(CentralFillFacility&)   │
│   : bool                            │
└─────────────────────────────────────┘
                    │
                    │ 1..*
                    │
                    ▼
┌─────────────────────────────────────┐
│      CentralFillFacilityManager     │
├─────────────────────────────────────┤
│ - facilities: vector<               │
│   CentralFillFacility>              │
├─────────────────────────────────────┤
│ + addFacility(CentralFillFacility&) │
│   : void                            │
│ + getFacilities():                  │
│   vector<CentralFillFacility>&      │
│ + findTop3Facilities(pair<int,int>) │
│   : vector<CentralFillFacility>     │
│ + findCheapestMedication(           │
│   CentralFillFacility&):            │
│   pair<string, float>               │
│ - calculateDistance(pair<int,int>,  │
│   pair<int,int>): int               │
└─────────────────────────────────────┘
```

## Relationship Details:

### **Composition Relationship**
- `CentralFillFacilityManager` **contains** multiple `CentralFillFacility` objects
- Multiplicity: 1 to many (1..*)
- The manager owns and manages the lifecycle of facilities

### **Method Visibility Legend**
- `+` = Public methods
- `-` = Private methods/attributes

### **Key Design Patterns**
1. **Encapsulation**: Private data members with public getters/setters
2. **Composition**: Manager contains and manages facility objects
3. **Single Responsibility**: Each class has a clear, focused purpose

### **Data Flow**
1. `main()` creates `CentralFillFacilityManager`
2. Multiple `CentralFillFacility` objects are created and added to manager
3. User provides location coordinates
4. Manager calculates distances and returns top 3 closest facilities
5. Manager finds cheapest medication for each facility

### **Method Interactions**
- `findTop3Facilities()` uses private `calculateDistance()`
- `findCheapestMedication()` accesses facility's prices and medications
- `setDistance()` allows manager to store calculated distances in facilities
