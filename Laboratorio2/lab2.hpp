#pragma once
#include <concepts>
#include <vector>
#include <string>
#include <functional>
#include <tuple>

template <typename t>
struct Eq;

template <typename t>
concept EqImplementation = requires (t x, t y)
  {
    { x == y } -> std::same_as<bool>;
    { x != y } -> std::same_as<bool>;
  };

template<>
struct Eq<int>
  { static bool eq(int x, int y) { return x == y; }
    static bool neq(int x, int y) { return x != y; }
  };

template <>
struct Eq<double>
  { static bool eq(double x, double y) { return x == y; }
    static bool neq(double x, double y) { return x != y; }
  };

template<>
struct Eq<std::string>
  { static bool eq(std::string x, std::string y) { return x == y; }
    static bool neq(std::string x, std::string y) { return x != y; }
  };

template<>
struct Eq<bool>
  { static bool eq(bool x, bool y) { return x == y; }
    static bool neq(bool x, bool y) { return x != y; }
  };


template <typename t>
struct Show;

template <typename t>
concept ShowImplementation = requires (t x)
  {
    { Show<t>::show(x) } -> std::same_as<std::string>;
  };

template<>
struct Show<int>
  { static std::string show(int x) { return std::to_string(x); } };

template <>
struct Show<double>
  { static std::string show(double x) { return std::to_string(x); } };

template<>
struct Show<std::string>
  { static std::string show(std::string x) { return x; } };

template<>
struct Show<bool>
  { static std::string show(bool x) { return x ? "true" : "false"; } };


template <typename t>
class List
  {
    private:
      std::vector<t> _inner;
      List<t> clone() const
      { List<t> xs;
        xs._inner = this->_inner;
        return xs;
      }
    public:
      List() {}
      List(List<t> const& other) : _inner(other._inner)  {}
      List<t> cons (t x) const
      { List<t> xs(*this);
        xs._inner.push_back(x);
        return xs;
      }
      List<t> tail() const
      { List<t> xs(*this);
        if (xs._inner.size() > 0)
          xs._inner.pop_back();
        return xs;
      }
      t head() const
      { return _inner.back(); }
      bool isEmpty() const
      { return _inner.empty(); }
  };

template<typename a, typename b>
  requires ShowImplementation<a> && ShowImplementation<b>
struct Show<std::tuple<a, b>>
  { static std::string show(std::tuple<a, b> t)
    { return "(" + Show<a>::show(std::get<0>(t)) + ", " + Show<b>::show(std::get<1>(t)) + ")"; }
  };


template <typename t>
  requires ShowImplementation<t>
struct Show<List<t>>
  { static std::string show(List<t> xs)
    { std::string s = "[";
      while(!xs.isEmpty())
        { s += Show<t>::show(xs.head());
          xs = xs.tail();
          if (!xs.isEmpty())
            s += ", ";
        }
      s += "]";
      return s;
    } };

template <typename t>
  requires EqImplementation<t>
struct Eq<List<t>>
  { static bool eq(List<t> xs, List<t> ys)
    { if (xs.isEmpty() && ys.isEmpty())
        return true;
      if (xs.isEmpty() || ys.isEmpty())
        return false;
      if (!Eq<t>::eq(xs.head(), ys.head()))
        return false;
      return eq(xs.tail(), ys.tail());
    } };


template <typename t>
  requires ShowImplementation<t>
std::string show(const t& x) {
    return Show<t>::show(x);
}

template <typename t>
  requires EqImplementation<t>
bool operator==(const List<t>& xs, const List<t>& ys) {
    return Eq<List<t>>::eq(xs, ys);
}

template <typename t>
  requires EqImplementation<t>
bool operator!=(const List<t>& xs, const List<t>& ys) {
    return !Eq<List<t>>::eq(xs, ys);
}

// --- FUNCIONES AUXILIARES ---

template<typename t>
List<t> reverseList(List<t> xs) {
    List<t> res;
    List<t> actual = xs;
    while (!actual.isEmpty()) {
        res = res.cons(actual.head());
        actual = actual.tail();
    }
    return res;
}

template<typename t>
int lengthList(List<t> xs) {
    int contador = 0;
    while (!xs.isEmpty()) {
        contador++;
        xs = xs.tail();
    }
    return contador;
}

// --- EJERCICIOS ---

// Ejercicio 1
template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeft(f_b_a_b f, b init, List<a> const& xs )
{
    b ans = init;
    List<a> it = xs;
    while (!it.isEmpty()) {
        b valor_temp = f(ans, it.head());
        ans = valor_temp;
        it = it.tail();
    }
    return ans;
}

// Ejercicio 2
template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeftRec(f_b_a_b f, b init, List<a> const& xs )
{
    if (xs.isEmpty()) {
        return init;
    } else {
        b nuevo_init = f(init, xs.head());
        return reduceLeftRec<a, b, f_b_a_b>(f, nuevo_init, xs.tail());
    }
}

// Ejercicio 3
template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> map(f_a_b f, List<a> const& xs )
{
    List<b> res;
    List<a> it = xs;
    while (!it.isEmpty()) {
        res = res.cons(f(it.head()));
        it = it.tail();
    }
    return reverseList(res);
}

// Ejercicio 4
template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapRec(f_a_b f, List<a> const& xs )
{
    if (xs.isEmpty()) {
        return List<b>();
    } else {
        List<b> resto_mapeado = mapRec<a, b, f_a_b>(f, xs.tail());
        return resto_mapeado.cons(f(xs.head()));
    }
}

// Ejercicio 5
template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapReduce(f_a_b f, List<a> const& xs )
{
    auto paso = [&](List<b> acc, a item) -> List<b> { 
        List<b> nuevo_acc = acc.cons(f(item));
        return nuevo_acc; 
    };
    List<b> lista_al_reves = reduceLeft<a, List<b>, decltype(paso)>(paso, List<b>(), xs);
    return reverseList(lista_al_reves);
}

// Ejercicio 6
template <typename t>
List<t> concat(List<List<t>> const& xss)
{
    auto unir = [](List<t> acc, List<t> sub) -> List<t> {
        List<t> acc_volteado = reverseList(acc);
        while (!acc_volteado.isEmpty()) {
            sub = sub.cons(acc_volteado.head());
            acc_volteado = acc_volteado.tail();
        }
        return sub;
    };
    return reduceLeft<List<t>, List<t>, decltype(unir)>(unir, List<t>(), xss);
}

// Ejercicio 7
template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filter(f_a_b f, List<a> const& xs )
{
    List<a> res;
    List<a> it = xs;
    while (!it.isEmpty()) {
        if (f(it.head()) == true) {
            res = res.cons(it.head());
        }
        it = it.tail();
    }
    return reverseList(res);
}

// Ejercicio 8
template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterRec(f_a_b f, List<a> const& xs )
{
    if (xs.isEmpty()) {
        return List<a>();
    }

    List<a> resto = filterRec<a, f_a_b>(f, xs.tail());

    if (f(xs.head())) {
        return resto.cons(xs.head());
    } else {
        return resto;
    }
}

// Ejercicio 9
template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterMap(f_a_b f, List<a> const& xs )
{
    auto f_a_lista = [&](a item) -> List<a> { 
        if (f(item)) {
            return List<a>().cons(item);
        }
        return List<a>();
    };
    List<List<a>> mapeado = map<a, List<a>, decltype(f_a_lista)>(f_a_lista, xs);
    return concat(mapeado);
}

// Ejercicio 10
template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterReduce(f_a_b f, List<a> const& xs )
{
    auto reductor = [&](List<a> acc, a val) -> List<a> { 
        if (f(val)) {
            return acc.cons(val);
        } else {
            return acc;
        }
    };
    List<a> al_reves = reduceLeft<a, List<a>, decltype(reductor)>(reductor, List<a>(), xs);
    return reverseList(al_reves);
}

// Ejercicio 11
template <typename t>
  requires EqImplementation<t>
List<List<t>> groupEqual(List<t> const& xs)
{
    auto agrupar_paso = [](List<List<t>> acc, t val) -> List<List<t>> {
        if (acc.isEmpty()) {
            List<t> nuevo_grupo;
            return acc.cons(nuevo_grupo.cons(val));
        }

        List<t> grupo_actual = acc.head();
        t valor_grupo = grupo_actual.head();

        if (Eq<t>::eq(valor_grupo, val)) {
            List<t> grupo_actualizado = grupo_actual.cons(val);
            return acc.tail().cons(grupo_actualizado);
        } else {
            List<t> siguiente_grupo;
            return acc.cons(siguiente_grupo.cons(val));
        }
    };
    List<List<t>> listas_al_reves = reduceLeft<t, List<List<t>>, decltype(agrupar_paso)>(agrupar_paso, List<List<t>>(), xs);
    return reverseList(listas_al_reves);
}

// Ejercicio 12
template <typename t>
  requires EqImplementation<t>
List<std::tuple<t, int>> encode(List<t> const& xs)
{
    List<List<t>> grupos = groupEqual(xs);
    auto hacer_par = [](List<t> g) -> std::tuple<t, int> {
        t elemento = g.head();
        int tamano = lengthList(g);
        return std::make_tuple(elemento, tamano);
    };
    return map<List<t>, std::tuple<t, int>, decltype(hacer_par)>(hacer_par, grupos);
}

// Ejercicio 13
template <typename t>
  requires EqImplementation<t>
List<t> decode(List<std::tuple<t, int>> const& xss)
{
    auto desempacar = [](std::tuple<t, int> p) -> List<t> {
        t elemento = std::get<0>(p);
        int contador = std::get<1>(p);
        List<t> sublista;

        int i = 0;
        while (i < contador) {
            sublista = sublista.cons(elemento);
            i++;
        }
        return sublista;
    };
    return concat(map<std::tuple<t, int>, List<t>, decltype(desempacar)>(desempacar, xss));
}

// Ejercicio 14
template <typename a, typename b>
List<std::tuple<a,b>> zip(List<a> const& xs, List<b> const& ys)
{
    List<a> it_x = xs;
    List<b> it_y = ys;
    List<std::tuple<a,b>> res;

    while (true) {
        if (it_x.isEmpty() || it_y.isEmpty()) {
            break; 
        }
        res = res.cons(std::make_tuple(it_x.head(), it_y.head()));
        it_x = it_x.tail();
        it_y = it_y.tail();
    }
    return reverseList(res);
}

// Ejercicio 15
template <typename t>
List<t> dropEveryN(int n, List<t> const& xs)
{
    if (n <= 0) return xs;

    int tamano_total = lengthList(xs);
    List<int> numeros;
    int k = tamano_total;

    while (k > 0) {
        numeros = numeros.cons(k);
        k--;
    }

    List<std::tuple<t, int>> pares = zip(xs, numeros);

    auto cond_guardar = [n](std::tuple<t, int> p) -> bool { 
        int indice = std::get<1>(p);
        return (indice % n) != 0; 
    };
    List<std::tuple<t, int>> pares_guardados = filter<std::tuple<t, int>, decltype(cond_guardar)>(cond_guardar, pares);

    auto sacar_primero = [](std::tuple<t, int> p) -> t { 
        return std::get<0>(p); 
    };
    return map<std::tuple<t, int>, t, decltype(sacar_primero)>(sacar_primero, pares_guardados);
}

// Helper para Ejercicio 16
template <typename t, typename f_a_b>
List<t> insertSorted(List<t> lista_ordenada, t nuevo_val, f_a_b f) {
    if (lista_ordenada.isEmpty()) {
        return lista_ordenada.cons(nuevo_val);
    }

    t cabeza_actual = lista_ordenada.head();
    if (f(nuevo_val, cabeza_actual)) {
        return lista_ordenada.cons(nuevo_val);
    } else {
        List<t> resto_insertado = insertSorted(lista_ordenada.tail(), nuevo_val, f);
        return resto_insertado.cons(cabeza_actual);
    }
}

// Ejercicio 16
template <typename t, typename f_a_b>
  requires std::invocable<f_a_b, t, t>
    && std::convertible_to<f_a_b,std::function<bool(t, t)>>
List<t> sortBy(List<t> const& xs, f_a_b f)
{
    List<t> final_ordenado;
    List<t> it = xs;
    while (!it.isEmpty()) {
        final_ordenado = insertSorted(final_ordenado, it.head(), f);
        it = it.tail();
    }
    return final_ordenado;
}

// Ejercicio 17
template <typename t>
List<t> rotateByN(List<t> const& xs, int n)
{
    if (xs.isEmpty()) {
        return xs;
    }

    int size = lengthList(xs);
    int n_real = ((n % size) + size) % size; 

    List<int> indices;
    int i = size - 1;
    while (i >= 0) {
        indices = indices.cons(i);
        i--;
    }

    List<std::tuple<t, int>> lista_zipeada = zip(xs, indices);

    auto func_orden = [n_real, size](std::tuple<t, int> a, std::tuple<t, int> b) -> bool {
        int nueva_pos_a = (std::get<1>(a) + size - n_real) % size;
        int nueva_pos_b = (std::get<1>(b) + size - n_real) % size;
        return nueva_pos_a <= nueva_pos_b;
    };
  List<std::tuple<t, int>> pares_ordenados = sortBy<std::tuple<t, int>, decltype(func_orden)>(lista_zipeada, func_orden);

      auto extraer_val = [](std::tuple<t, int> p) -> t { 
          return std::get<0>(p); 
      };
      return map<std::tuple<t, int>, t, decltype(extraer_val)>(extraer_val, pares_ordenados);
  }
