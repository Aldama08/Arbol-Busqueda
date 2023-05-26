#ifndef ArbolB_H_INCLUDED
#define ArbolB_H_INCLUDED
template <class T>
class ArbolB {
    private:
        class Nodo {
            private:
                T* dataPtr;
                Nodo* next;
                Nodo* prev;
            public:
                class Exception : public std::exception {
                    private:
                        std::string msg;
                    public:
                        explicit Exception(const char* message) : msg(message) {}
                        explicit Exception(const std::string& message) : msg(message) {}
                        virtual ~Exception()throw() {}
                        virtual const char* what() const throw () {
                            return msg.c_str();
                            }
                    };
                Nodo();
                Nodo(const T&);
                ~Nodo();
                T* getDataPtr() const;
                T getData() const;
                Nodo*& getnext();
                Nodo*& getprev();
                void setDataPtr(T*);
                void setData(const T&);
                void setnext(Nodo*&);
                void setprev(Nodo*&);
            };
    public:
        typedef Nodo* Position;
    private:
        Position root;

        void copyAll(const ArbolB<T>&);
        void copyAll(Position&,Position&);
        void inserta(Position&, const T&);
        void elimina(Position&, const T&);
        Position& localiza(Position&, const T&);
        int comparar(const T&, const T&) const;
        Position& getLowest(Position&);
        Position& getHighest(Position&);

    public:
        class Exception : public std::exception {
            private:
                std::string msg;
            public:
                explicit Exception(const char* message) : msg(message) {}
                explicit Exception(const std::string& message) : msg(message) {}
                virtual ~Exception()throw() {}
                virtual const char* what() const throw () {
                    return msg.c_str();
                    }
            };
        ArbolB();
        ArbolB(const ArbolB&);

        ~ArbolB();

        bool isEmpty() const;
        void inserta(const T&);


        void elimina(Position&);


        T retrieve(Position&) const;

        int getHeight(Position&);
        int getRHeight(Position&);
        int getLHeight(Position&);

        Position& localiza(const T&);

        Position& getLowest();
        Position& getHighest();

        bool isLeaf(Position&) const;

        int getHeight();


        void deleteAll(Position&);
        void deleteAll();
        ArbolB& operator = (const ArbolB&);
    };


template <class T>
ArbolB<T>::Nodo::Nodo() : dataPtr(nullptr), next(nullptr), prev(nullptr) {}

template <class T>
ArbolB<T>::Nodo::Nodo(const T& e) : dataPtr(new T(e)), next(nullptr), prev(nullptr) {
    if(dataPtr == nullptr) {
        throw Exception("Memoria insuficiente, creando nodo");
        }
    }
template <class T>
ArbolB<T>::Nodo::~Nodo() {
    delete dataPtr;
    }
template <class T>
T* ArbolB<T>::Nodo::getDataPtr() const {
    return dataPtr;
    }
template <class T>
T ArbolB<T>::Nodo::getData() const {
    if(dataPtr == nullptr) {
        throw Exception("Dato inexistente, getData");
        }
    return *dataPtr;
    }
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::Nodo::getnext() {
    return next;
    }
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::Nodo::getprev() {
    return prev;
    }
template <class T>
void ArbolB<T>::Nodo::setDataPtr(T* da) {
    dataPtr = da;
    }
template <class T>
void ArbolB<T>::Nodo::setData(const T& e) {
    if(dataPtr == nullptr) {
        if((dataPtr = new T(e)) == nullptr) {
            throw Exception("Memoria no disponible, setData");
            }
        }
    else {
        *dataPtr = e;
        }
    }
template <class T>
void ArbolB<T>::Nodo::setprev(Nodo*& p) {
    prev = p;
}
template <class T>
void ArbolB<T>::Nodo::setnext(Nodo*& n) {
    next = n;
}


template <class T>
void ArbolB<T>::copyAll(Position& pos, Position& sol) {
    if (sol == nullptr) {
        pos = nullptr;
    } else {
        pos = new Nodo(sol->getData());

        pos->setnext(sol->getnext());  // Asignar puntero next
        pos->setprev(sol->getprev());  // Asignar puntero prev

        copyAll(pos->getnext(), sol->getnext());
        copyAll(pos->getprev(), sol->getprev());
    }
}


template <class T>
ArbolB<T>::ArbolB() : root(nullptr) {}
template <class T>
ArbolB<T>::ArbolB(const ArbolB& br) : root(nullptr) {
    copyAll(br);
    }
template <class T>
ArbolB<T>::~ArbolB() {
    deleteAll();
    }
template <class T>
bool ArbolB<T>::isEmpty() const {
    return root == nullptr;
    }
template <class T>
void ArbolB<T>::inserta(const T& e) {
    inserta(root, e);
}

template <class T>
void ArbolB<T>::inserta(Position& pos, const T& e) {
    if (pos == nullptr) {
        try {
            pos = new Nodo(e);
        } catch (typename Nodo::Exception ex) {
            throw Exception(ex.what());
        }
    } else {
        if (e < pos->getData()) {
            inserta(pos->getprev(), e);
            pos->getprev()->setnext(pos);  // Establecer puntero next del nodo anterior
        } else {
            inserta(pos->getnext(), e);
            pos->getnext()->setprev(pos);  // Establecer puntero prev del nodo siguiente
        }
    }
}





template <class T>
int ArbolB<T>::comparar(const T& a, const T& b) const {
    if (a < b) {
        return -1;
    } else if (b < a) {
        return 1;
    } else {
        return 0;
    }
}







template <class T>
void ArbolB<T>::elimina(Position& pos,const T& e) {
    pos = localiza(e);
    elimina(pos);
}

template <class T>
void ArbolB<T>::elimina(Position& r) {
    if (r == nullptr) {
        return;
    }

    if (isLeaf(r)) {
        delete r;
        r = nullptr;
        return;
    }

    if (r->getnext() != nullptr || r->getprev() == nullptr) {
        Position aux = r;
        r = r->getnext() == nullptr ? r->getprev() : r->getnext();
        delete aux;
        return;
    }

    T substitute = getHighest(r->getnext())->getData();
    r->setData(substitute);
    elimina(r->getnext(), substitute);
}


template <class T>
T ArbolB<T>::retrieve(Position& pos)const {
    return pos->getData();
    }
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::localiza(const T& e) {
    return localiza(root, e);
    }
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::localiza(Position& pos, const T& e) {
    if (pos == nullptr || pos->getData() == e) {
        return pos;
    }
    if (e < pos->getData()) {
        return localiza(pos->getprev(), e);
    }
    return localiza(pos->getnext(), e);
}


template <class T>
typename ArbolB<T>::Position& ArbolB<T>::getLowest() {
    return getLowest(root);
    }
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::getLowest(Position& pos) {
    if(pos == nullptr || pos->getnext() == nullptr) {
        return pos;
        }
    return getLowest(pos->getnext());
    }
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::getHighest() {
    return getHighest(root);
}
template <class T>
typename ArbolB<T>::Position& ArbolB<T>::getHighest(Position& pos) {
    if(pos == nullptr || pos->getprev() == nullptr) {
        return pos;
        }
    return getHighest(pos->getprev());
    }
template <class T>
bool ArbolB<T>::isLeaf(Position& pos)const {
    return pos != nullptr && pos->getnext() == pos->getprev();
    }
template <class T>
int ArbolB<T>::getHeight() {
    return getHeight(root);
    }
template <class T>
int ArbolB<T>::getHeight(Position& pos) {
    if(pos == nullptr) {
        return 0;
        }
    int ho(getHeight(pos->getnext()));
    int ho2(getHeight(pos->getprev()));
    return (ho > ho2 ? ho : ho2) + 1;
    }


template <class T>
void ArbolB<T>::deleteAll() {
    deleteAll(root);
    }
template <class T>
void ArbolB<T>::deleteAll(Position& pos) {
    if(pos != nullptr) {
        deleteAll(pos->getnext());
        deleteAll(pos->getprev());
        delete pos;
        }
    }
template <class T>
ArbolB<T>& ArbolB<T>::operator=(const ArbolB& t) {
    deleteAll();
    copyAll(t);

    return *this;
    }

#endif // ArbolB_H_INCLUDED
