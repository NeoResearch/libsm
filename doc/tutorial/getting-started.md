1. define the state `S`, input `I` and output `O` of the state

   ```c++
   class S
   {
       // ...
   };
   class I
   {
       // ...
   };
   class O
   {
       // ...
   };
   ```

2. inherit the state machine class based on `sm::SM<S, I, O>` and override the transition function `tf` and output function `of` and function `goon`

   ```c++
   class StateMachine : public sm::SM<S, I, O>
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

3. create state machine instances in main function and run

   ```c++
   int main()
   {
       auto sm = StateMachine([] { return I::MSG; }, [](const O &) { std::cout << "ok" << std::endl; });
       sm.run(S::INIT);
       return 0;
   }
   ```
