# SO Multi-platform Development

-- Organizare
Solutia aleasa si implementata in programul meu consta in citirea secventiala 
a liniilor de input, verificarea daca acestea sunt directive sau text si 
afisarea sau stocarea acestora.
Pentru directiva #define am folosit un HashMap, iar pentru #include o lista
dublu inlantuita.
Pentru a se iesi din program atunci cand o functie de alocare de memorie 
esueaza am folosit macroul DIE.


-- Implementare
Programul nu functioneaza pentru multi-lines defines, in rest este functional. 


-- Compilare
Windows: nmake
Linux: make


-- Rulare
Windows: ./so-cpp.exe <lista_parametrii>
Linux: ./so-cpp <lista_parametrii>
