1. define the state, input and output of the state

   ```c++
   class State
   {
       // ...
   };
   class Input
   {
       // ...
   };
   class Output
   {
       // ...
   };
   ```

2. inherit the state machine class based on `sm::SM<State, Input, Output>` and override the transition function `tf` and output function `of` and function `goon`

   ```c++
   class StateMachine : public sm::SM<State, Input, Output>
   {
       virtual S tf(const I &i, const S &s) override
       {
           // ...
       }
       virtual O of(const I &i, const S &s) override
       {
           // ...
       }
       virtual bool goon(const S &s) override
       {
           // ...
       }
   };
   ```

3. inherit the context class based on `sm::SM<Input, Output>` and override the function `send`

   ```c++
   class Context : public sm::CTX<Input, Output>
   {
       virtual void send(void *sm, const O &o) override
       {
           // ...
       }
   }
   ```

4. main function

   ```c++
   int main()
   {
       // create some instanse of state machines and an instanse of context
       // run each state machine in parallel
       // you may send some input to trig some state machines
       return 0;
   }
   ```