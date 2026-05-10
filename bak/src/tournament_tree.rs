#![allow(unused)]

use core::panic;
use std::rc::Rc;
use std::cell::RefCell;

#[derive(Debug)]
struct Cell<T>(Rc<RefCell<Option<T>>>);

impl<T> Cell<T> {
    fn new(v: Option<T>) -> Self {
        Self(Rc::new(RefCell::new(v)))
    }
}

impl<T> Clone for Cell<T> {
    fn clone(&self) -> Self {
        Self(self.0.clone())
    }
}

#[derive(Debug)]
struct Node<T: Ord> {
    parent: Cell<Node<T>>,
    leaf: Cell<Leaf<T>>,
    left_child: Option<Child<T>>,
    right_child: Option<Child<T>>,
}

#[derive(Debug)]
struct Leaf<T: Ord> {
    parent: Cell<Node<T>>,
    value: T,
    node: Cell<Node<T>>,
}

#[derive(Debug)]
enum Child<T: Ord> {
    Node(Cell<Node<T>>),
    Leaf(Cell<Leaf<T>>),
}

impl<T: Ord> Child<T> {
    fn leaf(&self) -> Cell<Leaf<T>> {
        match self {
            Child::Node(node) => {
                let reference = node.0.borrow();
                let node = reference.as_ref().expect(msg)

                panic!();
            },
            Child::Leaf(leaf) => leaf.clone(),
        }
    }
}

#[derive(Debug)]
struct TournamentTree<T: Ord> (Cell<Child<T>>);

impl<T: Ord> TournamentTree<T> {
    fn new(v: impl IntoIterator<Item = T>) -> Option<Self> {
        let mut queue = std::collections::VecDeque::<Cell<Child<T>>>::new();

        for v in v.into_iter() {
            let leaf = Leaf {
                parent: Cell::new(None),
                value: v,
                node: Cell::new(None),
            };

            let child = Cell::new(Some(Child::Leaf(leaf)));
            queue.push_back(child);
        }

        if queue.is_empty() {
            return None;
        }

        while queue.len() >= 2 {
            let l = queue.pop_front().expect("queue to have at least 1 element");
            let r = queue.pop_front().expect("queue to have at least 2 elements");

            let leaf = {
                let leaf = &l.leaf().0;
                let reference = leaf.borrow();
                let lhs = &reference.as_ref().expect("l to have a value").value;

                let leaf = &r.leaf().0;
                let reference = leaf.borrow();
                let rhs = &reference.as_ref().expect("r to have a value").value;

                if lhs < rhs {
                    l.leaf().clone()
                } else {
                    r.leaf().clone()
                }
            };

            let node = Node {
                parent: Cell::new(None),
                leaf,
                left_child: l,
                right_child: r,
            };
            let child = Cell::new(Some(Child::Node(node)));
            queue.push_back(child);
        }

        let root = queue.pop_front().expect("queue to have 1 element");
        Some(Self(root))
    }
}

pub fn test() {
    let tree = TournamentTree::new([7, 4, 9, 10, 12, 0]).unwrap();
    //println!("tree: {:?}", tree);
}
