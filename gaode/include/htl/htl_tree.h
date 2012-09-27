
/*
 * htl-lite - a basic data structures and algorithms.
 * Copyright (C) 2009 leon hong. All rights reserved.
 * authors:
 *   leon hong <codeblocker@gmail.com>
 */

#ifndef __HTL_TREE_H__
#define __HTL_TREE_H__

#include "htl_alloc.h"
#include "htl_pair.h"
#include "htl_algo.h"
//#include <iostream>

__HTL_BEGIN_NAMESPACE

typedef bool rb_tree_color_type;
const rb_tree_color_type rb_tree_red = false;   // ��ɫΪ 0
const rb_tree_color_type rb_tree_black = true;  // ��ɫΪ 1

// ������ڵ��base
struct rb_tree_node_base {
    rb_tree_color_type color;       // �ڵ���ɫ
    rb_tree_node_base* parent;      // ���ڵ�
    rb_tree_node_base* left;        // ����
    rb_tree_node_base* right;       // �Һ���

    // ������С�ڵ�
    static rb_tree_node_base* minimum(rb_tree_node_base* x) {
        while (x->left != NULL)
            x = x->left;
        return x;
    }

    // �������ڵ�
    static rb_tree_node_base* maximum(rb_tree_node_base* x) {
        while (x->right != NULL)
            x = x->right;
        return x;
    }
};

// ������ڵ�
template <class Value>
struct rb_tree_node : public rb_tree_node_base 
{
    typedef rb_tree_node<Value>* link_type;
    Value value_filed;  // �ڵ�ֵ
};

//RB-TREE��iterator��base
struct rb_tree_base_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef rb_tree_node_base* base_ptr;
    typedef ptrdiff_t difference_type;

    // ��������ָ��node
    base_ptr node;

    // ++����
    void increment() {
        if (node->right != NULL) {
            node = node->right;
            while (node->left != NULL)
                node = node->left;
        }
        else {
            base_ptr y = node->parent;
            // ���node��y���Һ��ӣ���һֱ����
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)   // ���������yΪroot����û��
                node = y;           // �Һ���(Ҳ����˵++֮��nodeָ��end()),��header
        }
    }

    // --����
    void decrement() {
        if (node->color == rb_tree_red          // nodeָ��end()ʱ(header), ��header��
            && node->parent->parent == node)    // �Һ���(Ҳ�������һ���ڵ�)����node
            node = node->right;
        else if (node->left != NULL) {
            node = node->left;
            while (node->right != NULL)
                node = node->right;
        }
        else {
            base_ptr y = node->parent;
            // ���node��y�����ӣ���һֱ����
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

// RB-TREE��iterator
template<class Value, class Ref, class Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef rb_tree_iterator<Value, Ref, Ptr> self;
    typedef rb_tree_node<Value>* link_type;

    // ���캯��
    rb_tree_iterator() {}
    rb_tree_iterator(link_type x){
        node = x;
    }
    rb_tree_iterator(const iterator& it) {
        node = it.node;
    }

    //�����ǵ������ṩ����ָ�������operator
    reference operator*() const {
        return link_type(node)->value_filed;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self& operator++() {
        increment();
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--() {
        decrement();
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }

};

inline bool operator==(const rb_tree_base_iterator& x,
                       const rb_tree_base_iterator& y) {
  return x.node == y.node;
}

inline bool operator!=(const rb_tree_base_iterator& x,
                       const rb_tree_base_iterator& y) {
  return x.node != y.node;
}

// RB-TREE
template<class Key,
         class Value,
         class KeyOfValue,
         class Compare,
         class Alloc = allocator>
class rb_tree {
protected:
    typedef rb_tree_node_base* base_ptr;
    typedef rb_tree_node<Value> _rb_tree_node;
    typedef simple_alloc<_rb_tree_node, Alloc> rb_tree_node_allocator;
    typedef rb_tree_color_type color_type;
public:
    typedef Value value_type;
    typedef Key key_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef _rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef rb_tree_iterator<value_type, reference, pointer> iterator;
protected:
    // ����һ��rb_node�ռ�
    link_type get_node() {
        return rb_tree_node_allocator::allocate();
    }

    // �ͷ�һ��rb_node�ռ�
    void put_node(link_type p) {
        return rb_tree_node_allocator::deallocate(p);
    }

    // ����ֵΪx��node
    link_type create_node(const value_type& x) {
        link_type tmp = get_node();
        construct(&tmp->value_filed, x);
        return tmp;
    }

    // �ͷ�node���ͷ�+������
    void destory_node(link_type p) {
        destory(&p->value_filed);
        put_node(p);
    }

    // ��¡һ��node��ֵ
    link_type clone_node(link_type x) {
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = 0;
        tmp->right = 0;
        tmp->parent = 0;
        return tmp;
    }
protected:
    // ����ΪRB-TREE�����ݳ�Ա
    size_type node_count;   // node�ڵ����
    link_type header;       // header���ڵ�ָ��root,���Һ��ӷֱ�ָ��������������Ҷ�
    Compare key_compare;    // Key function obj
protected:
    // ȡ��root�ڵ�
    link_type& root() const {
        return (link_type&) header->parent;
    }

    // ȡ��header�����ӣ�������ڵ㣩
    link_type& leftmost() const {
        return (link_type&) header->left;
    }

    // ȡ��header���Һ��ӣ�������ڵ㣩
    link_type& rightmost() const {
        return (link_type&) header->right;
    }

    // ����6�麯��(���link_type)����ȡ�ýڵ�x�ĸ�����
    static link_type& left(link_type x) {
        return (link_type&) (x->left);
    }

    static link_type& right(link_type x) {
        return (link_type&) (x->right);
    }

    static link_type& parent(link_type x) {
        return (link_type&) (x->parent);
    }

    static reference value(link_type x) {
        return x->value_field;
    }

    static const key_type& key(link_type x){
        return (key_type&)(x->value_filed);
    }

    static color_type& color(link_type x) {
        return (color_type&) (x->color);
    }

    // ����6�麯��(���base_ptr)����ȡ�ýڵ�x�ĸ�����
    static link_type& left(base_ptr x) {
        return (link_type&) (x->left);
    }

    static link_type& right(base_ptr x) {
        return (link_type&) (x->right);
    }

    static link_type parent(base_ptr x) {
        return (link_type&) (x->parent);
    }

    static reference value(base_ptr x) {
        return ((link_type)x)->value_filed;
    }

    static const key_type& key(base_ptr x) {
        return (key_type&)(value(x));
    }

    static color_type& color(base_ptr x) {
        return (color_type&) (x->color);
    }

    // ��Сֵָ���node
    static link_type minimum(link_type x) {
        return (link_type) rb_tree_node_base::minimum(x);
    }

    // ���ֵָ���node
    static link_type maximum(link_type x) {
        return (link_type&) rb_tree_node_base::maximum(x);
    }
private:
    // ��ʼ��
    void init() {
        header = get_node();
        color(header) = rb_tree_red;    // ���ú�ɫ������root��header����decrement�����֣�
        root() = 0;
        leftmost() = header;            // ��ʼ��header����ָ���Լ�
        rightmost() = header;           // ��ʼ��header�Һ���ָ���Լ�
    }
public:
    rb_tree(const Compare& comp = Compare())
     : node_count(0), key_compare(comp) {
        init();
    }
    ~rb_tree() {
        clear();
        put_node(header);
    }

	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& 
		operator=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& x);
public:
    Compare key_comp() const {
        return key_compare;
    }

    // begin()Ϊheader������
    iterator begin() {
        return leftmost();
    }

    // endΪheader
    iterator end() {
        return header;
    }

    bool empty() const {
        return node_count == 0;
    }

    size_type size() const {
        return node_count;
    }

    // �õ�root�ڵ�
    link_type get_root() {
        return root();
    }
    // �������
    void mid_visit(link_type p);

    // ��x���뵽RB-TREE�У��ڵ�����ظ�
    iterator insert_equal(const value_type& v);

    // ��x���뵽RB-TREE�У��ڵ㲻�����ظ�
    pair<iterator, bool> insert_unique(const value_type& v);

    // ɾ����������ָ��node
    void erase(iterator pos);

    // ɾ��keyֵΪx�Ľڵ�
    size_type erase(const Key& x);

    // ɾ��first~last֮���node
    void erase(iterator first, iterator last);

    // ����keyֵ
    iterator find(const key_type& k);

    // ɾ�����нڵ㣨������herader,�ص���ʼ��״̬��
    void clear();

    // ����keyֵΪx�ĵķ�Χ
    pair<iterator,iterator> equal_range(const key_type& x);

    // ���ص�һ�����ڵ���x��ֵ
    iterator lower_bound(const key_type& x);

    // ���ؿɲ���x�����һ��λ��
    iterator upper_bound(const key_type& x);

    // ����keyֵΪx�ĸ���
    size_type count(const Key& k);
private:
    // x�ǲ���㣬y��x�ĺ��ӷ�ڵ�,����ֵΪv
    iterator insert(link_type x, link_type y, const value_type& v);

    // insert�ڵ�z֮��,��Ҫ�������μ��ڵ���ɫ��ά�ֺ�����ṹ
    void insert_fixup(base_ptr z);

    // ɾ���ڵ�֮����Ҫ�������μ��ڵ���ɫ��ά�ֺ�����ṹ
    void erase_fixup(base_ptr x_parent, base_ptr x);

    // ����ת
    void left_rotate(base_ptr x);

    // ����ת
    void right_rotate(base_ptr y);

    // �ڲ�ʹ��erase_aux,�����Ǻ������£�ֱ��ɾ�����нڵ�
    void erase_aux(link_type x);

    // ���x�ĺ��
    base_ptr tree_successor(base_ptr x);

    // ��ӡValue
    void print_node(link_type p) {
        //std::cout << p <<" Value : " <<
        //    p->value_filed << " " << "color : ";
        //std::cout << (p->color ?  "black" : "red") << std::endl;
    }
};


template <class Key, class Value, class KeyOfValue, class Compare,
class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& 
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>
	::operator=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& x)
{
	if (this != &x) {
		// Note that _Key may be a constant type.
		clear();
		node_count = 0;
		key_compare = x.key_compare;        
		if (x.root() == 0) {
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}
		else {
			root() = copy(x.root(), header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
			node_count = x.node_count;
		}
	}
	return *this;
}

template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::mid_visit(link_type p) {
    if (p == NULL) {
        //std::cout << "has no node\n";
        return;
    }
	if (p->left) {
		if (((link_type)p->left)->value_filed > ((link_type)p)->value_filed)
			//std::cout << "wrong left\n";
		//assert(((link_type)p->left)->value_filed <= ((link_type)p)->value_filed);
		mid_visit((link_type)p->left);
	}
	print_node(p);
	if (p->right) {
		if (((link_type)p->right)->value_filed < ((link_type)p)->value_filed)
			//std::cout << "wrong right\n";
		//assert(((link_type)p->right)->value_filed >= ((link_type)p)->value_filed);
		mid_visit((link_type)p->right);
	}
}
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert_equal(const value_type& v) {
    link_type y = header;
    link_type x = root();
    while (x != NULL) {
		key_type aKeyV = KeyOfValue()(v);
		key_type aKeyX = key(x);

        y = x;
        x = key_compare(aKeyV, aKeyX) ? left(x) : right(x);
    }
    return insert(x, y, v);
}

template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert_unique(const value_type& v) {
    link_type y = header;
    link_type x = root();
    bool comp = true;   // ��ʶ�����뻹���Ҳ����
    while (x != NULL) {
        y = x;
		key_type aKeyValue = KeyOfValue()(v);
		key_type aKeyValueX = key(x);
        comp = key_compare(aKeyValue, aKeyValueX);
        x = comp ? left(x) : right(x);
    }
    iterator it = iterator(y);  // �����ĸ��ڵ�
    if (comp) {
        // ��ʾ��y�������룬��Ϊ�������:(1). �����������ֱ�Ӳ���.
        // (2).���򣬻���(��Ϊ�������룬���Ը�ֵһ��С��y,����ֻ��Ҫ�ͱȽϵ�һ��yС��ֵ�Ƚϼ���)
        if (it == begin())
            return pair<iterator, bool>(insert(x, y, v), true);
        else
            --it;
    }
    // compΪ�٣��Ҳ����
	key_type aKeyValueNode = key(it.node);
	key_type aKeyValueV = KeyOfValue()(v);

    if (key_compare(aKeyValueNode, aKeyValueV))  // �������ֵ��it��ֵ�����(it����һ�����ڵ��ڲ���ֵ��ֵ)
        return pair<iterator, bool>(insert(x, y, v), true);
    // ���������Ϊ��Ϊ�ظ�
    return pair<iterator, bool>(it, false);
}
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert(link_type x, link_type y, const value_type& v) {
    link_type z = create_node(v);
	key_type aKeyValueV = KeyOfValue()(v);
	key_type aKeyY = key(y);

	/*link_type z = create_node(v);
	key_type aKeyValueV = KeyOfValue()(v);
	key_type aKeyValueX;
	if (x != NULL)
	{
		aKeyValueX = key(x);
	}
	
    if (y == header) {
        // �����header
        root() = z;
        left(y) = z;
        right(y) = z;
    }
    else if (key_compare(aKeyValueV, aKeyValueX)){
        // ���vС��y,����y�����Ӳ���
        left(y) = z;
        // �Ƿ���Ҫ����leftmost
        if (y == leftmost())
            leftmost() = z;
    }*/
	if (y == header || x != NULL || 
		key_compare(aKeyValueV, aKeyY)) {
			left(y) = z;               // also makes _M_leftmost() = __z 
			//    when __y == _M_header
			if (y == header) {
				root() = z;
				rightmost() = z;
			}
			else if (y == leftmost())
				leftmost() = z;   // maintain _M_leftmost() pointing to min node
	}
    else {
        // v����y����y���Һ��Ӳ���
        right(y) = z;
        // �Ƿ���Ҫ����rightmost
        if (y == rightmost())
            rightmost() = z;
    }
    parent(z) = y;
    left(z) = 0;
    right(z) = 0;
    // �����ڵ���Ϊ��
    color(z) = rb_tree_red;
    // ������ɫ����ά��RB-TREE����
    insert_fixup(z);
    ++node_count;
    // ���ز���ڵ��iterator
    return iterator(z);
}

/**********************************************************
	����ڵ�z֮����Ҫ������ɫ�����ֺ��������
	����3�������
	(1). z�������Ǻ�ɫ
	(2). z�������Ǻ�ɫ������z���Һ���(���2����Ϊ���3һ����)
	(3). z�������Ǻ�ɫ������z������
**********************************************************/
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert_fixup(base_ptr z) {
    while (z != root() && z->parent->color == rb_tree_red) {
		if (z->parent == z->parent->parent->left) {
			base_ptr y = z->parent->parent->right;
			if (y && y->color == rb_tree_red) {	// ���z������yΪ�� (����1)
				z->parent->color = rb_tree_black;
				y->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				z = z->parent->parent; // ����z��Ϊһ�´ε���
			}
			else { // ���z������yΪ��
				if (z == z->parent->right) { // ��תz�ĸ��ף�ʹ������㣨����3�����˲��ǽ�����2�������3һ����
					z = z->parent;
					left_rotate(z);
				}
				// ����3
				z->parent->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				right_rotate(z->parent->parent);
			}
		}
		else if (z->parent->parent != NULL) { // z->p == z->p->p->right
			base_ptr y = z->parent->parent->left;
			if (y && y->color == rb_tree_red) {
				z->parent->color = rb_tree_black;
				y->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					right_rotate(z);
				}
				z->parent->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				left_rotate(z->parent->parent);
			}
		}
	} // end while
	root()->color = rb_tree_black;
}

/**********************************************************
                    ��X����ת
        X                            Y
      /   \                        /   \
     a     Y           ====>      X     c
         /   \                  /   \
        b     c                a      b
**********************************************************/
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::left_rotate(base_ptr x) {
    base_ptr y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x == root()) {
		root() = (link_type)y;
	}
	else {
		if (x == x->parent->left)
			x->parent->left = y;
		else // x == x->p->right
			x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

/**********************************************************
                    ��y����ת
        X                            Y
      /   \                        /   \
     a     Y           <====      X     c
         /   \                  /   \
        b     c                a      b
**********************************************************/
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::right_rotate(base_ptr y) {
	base_ptr x = y->left;
	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;
	x->parent = y->parent;
	if (y == root()) {
		root() = (link_type)x;
	}
	else {
		if (y == y->parent->right)
			y->parent->right = x;
		else
			y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}

/**********************************************************
	��һ��x�ڵ�ĺ��
**********************************************************/
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::tree_successor(base_ptr x) {
    if (x->right != NULL)
		return rb_tree_node_base::minimum(x->right);
	base_ptr y = x->parent;
	while (y != root() && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

/**********************************************************
	ɾ��������Ϊ3�����
	(1). z���ӽڵ�
	(2). z��һ���ӽڵ�
	(3). z�������ӽڵ�
**********************************************************/
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::erase(iterator pos) {
    base_ptr z = pos.node;
	base_ptr x_parent = NULL;
	base_ptr y = NULL;
	if (z->left == NULL || z->right == NULL)
		y = z;
	else
		y = tree_successor(z);
	base_ptr x = NULL;
	if (y->left != NULL)
		x = y->left;
	else
		x = y->right;
	if (x != NULL)
		x->parent = y->parent;
	if (y == root()) {
		root() = (link_type) x;
		x_parent = header;
	}
	else {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
		x_parent = y->parent;
	}
	if (y != z) {
		((link_type)z)->value_filed = ((link_type)y)->value_filed;
	}
	else {  // y == z
        if ((((base_ptr)(header))->left) == y)  // ���Ҫɾ���Ľڵ�y��leftmost
            if (y->right == NULL)   // ��ʱy->leftҲһ��Ϊ�գ���Ϊ�����Ϊ�գ���leftmost != y
                ((base_ptr)(header))->left = y->parent;
            else    // y->right != NULL
                ((base_ptr)(header))->left = rb_tree_node_base::minimum(x);
        if ((((base_ptr)(header))->right) == y)
            if (y->left == NULL)
                ((base_ptr)(header))->right = y->parent;
            else
                ((base_ptr)(header))->right = rb_tree_node_base::maximum(x);
    }
	if (y->color == rb_tree_black)	// �����ɾ�����Ǻ�ɫ����Ҫ������ɾ������ӽڵ�
        erase_fixup(x_parent, x);
    destory_node((link_type)y);
    --node_count;
}

/***********************************************************
	ɾ��y֮����Ҫ����y���ӽڵ�x�������ֺ��������
	4����� :
	(1). x���ֵ�w�Ǻ�ɫ
	(2). x���ֵ�w�Ǻ�ɫ,����w�����������Ǻ�ɫ
	(3). x���ֵ�w�Ǻ�ɫ,w�������Ǻ�ɫ�ģ��Һ����Ǻ�ɫ��
	(4). x���ֵ�w�Ǻ�ɫ,w���Һ����Ǻ�ɫ
***********************************************************/
template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::erase_fixup(base_ptr x_parent, base_ptr x) {
    while (x != root() && (x == NULL || x->color == rb_tree_black)) {
		if (x == x_parent->left) {
			base_ptr w = x_parent->right;
			if (w->color == rb_tree_red) {	// ���(1)
				w->color = rb_tree_black;
				x_parent->color = rb_tree_red;
				left_rotate(x_parent);
				// �����ֵܽڵ�
				w = x_parent->right;
			}
			if ((w->left == NULL || w->left->color == rb_tree_black)
				&& (w->right == NULL || w->right->color == rb_tree_black)) { // ���(2)
				w->color = rb_tree_red;
				x = x_parent;	// ����x ��x�ĸ��ڵ�
				x_parent = x_parent->parent;
			}
			else {
				if (w->right == NULL || w->right->color == rb_tree_black) { // ���(3)
					w->left->color = rb_tree_black;
					w->color = rb_tree_red;
					right_rotate(w);
					w = x_parent->right;
				}
				// ���(4)
				w->color = x_parent->color;
				x_parent->color = rb_tree_black;
				w->right->color = rb_tree_black;
				left_rotate(x_parent);
				x = root();
			}
		}
		else { // x == x_parent->right
			base_ptr w = x_parent->left;
			if (w->color == rb_tree_red) {	// ���(1)
				w->color = rb_tree_black;
				x_parent->color = rb_tree_red;
				right_rotate(x_parent);
				// �����ֵܽڵ�
				w = x_parent->left;
			}
			if ((w->right == NULL || w->right->color == rb_tree_black)
				&& (w->left == NULL || w->left->color == rb_tree_black)) { // ���(2)
				w->color = rb_tree_red;
				x = x_parent;	// ����x ��x�ĸ��ڵ�
				x_parent = x_parent->parent;
			}
			else {
				if (w->left == NULL || w->left->color == rb_tree_black) { // ���(3)
					w->right->color = rb_tree_black;
					w->color = rb_tree_red;
					left_rotate(w);
					w = x_parent->left;
				}
				// ���(4)
				w->color = x_parent->color;
				x_parent->color = rb_tree_black;
				w->left->color = rb_tree_black;
				right_rotate(x_parent);
				x = root();
			}
		}
	}
	if (x != NULL)
		x->color = rb_tree_black;
}

template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::erase_aux(link_type x) {
    while (x != NULL) {
        // ��Ϊ�Һ��Ӻ�������ͬ�����£�д��ɾ�����ӹ��̣��ݹ��Һ��Ӽ���
        erase_aux(right(x));
        link_type y = left(x);
        destory_node(x);
        x = y;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first,
                                                            iterator last) {
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
    pair<iterator,iterator> p = equal_range(x);
    size_type n = 0;
    distance(p.first, p.second, n);
    erase(p.first, p.second);
    return n;
}

template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::clear() {
    if (node_count != 0) { // ����Ѿ�û�нڵ���(����header),�Ͳ�ɾ��
        erase_aux(root());
        leftmost() = header;
        root() = 0;
        rightmost() = header;
        node_count = 0;
    }
}

template<class Key, class Value, class KeyOfValue, class Compare,
         class Alloc>
typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::find(const key_type& k) {
    link_type y = header;   // y�洢���һ�����ڵ���kֵ�ýڵ�
    link_type x = root();
    while (x != NULL) {
    //key_type m = key(x);
    //key_type n = key(y);
        if (!key_compare(key(x), k)) // x��key >= k
            y = x, x = left(x);
        else    // x��key < k
            x = right(x);
    }
    //key_type aKey = key(y);
    iterator it = iterator(y);
    //aKey = key(it.node);
	key_type aKeyIt = key(it.node);

    return (it == end() || key_compare(k, aKeyIt)) ? end() : it;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) {
    link_type y = header; // y�洢���һ�����ڵ���kֵ�ýڵ�(>=)
    link_type x = root();
    while (x != 0)
	{
		key_type aKey = key(x);
		if (!key_compare(aKey, k))
			y = x, x = left(x);
		else
			x = right(x);
	}
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) {
    link_type y = header; // y�洢���һ������kֵ�ýڵ�(>)
    link_type x = root();
    while (x != 0)
	{
		key_type aKey = key(x);
		if (key_compare(k, aKey))
			y = x, x = left(x);
		else
			x = right(x);
	}
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const Key& k){
    pair<iterator, iterator> p = equal_range(k);
    size_type n = 0;
    distance(p.first, p.second, n);
    return n;
}

__HTL_END_NAMESPACE

#endif
