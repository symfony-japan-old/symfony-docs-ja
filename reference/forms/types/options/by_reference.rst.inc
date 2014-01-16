by_reference
~~~~~~~~~~~~

**データ型type**: ``Boolean`` **デフォルト**: ``true``

下層にあるオブジェクトに ``name`` フィールドを持つとして、ほとんどの場合、\ ``setName`` が呼ばれることを期待します。
しかし、場合によっては ``setName`` が呼ばれ *ない* ことがあります。\ ``by_reference`` をセットすることですべての場合にセッターが呼ばれることを確実にします。

さらに説明するため、簡単な例がこちらです。::

    $builder = $this->createFormBuilder($article);
    $builder
        ->add('title', 'text')
        ->add(
            $builder->create('author', 'form', array('by_reference' => ?))
                ->add('name', 'text')
                ->add('email', 'email')
        )

``by_reference`` が ``true`` の場合、フォームの ``submit`` （または ``handleRequest`` ）が呼ばれたとき、次のように裏側では処理されます。::

    $article->setTitle('...');
    $article->getAuthor()->setName('...');
    $article->getAuthor()->setEmail('...');

``setAuthor`` が呼び出されないことに注意して下さい。その ``author`` はリファレンスにより変更されます。

``by_reference`` が ``false`` の場合、送信時の処理はこのようになります。::

    $article->setTitle('...');
    $author = $article->getAuthor();
    $author->setName('...');
    $author->setEmail('...');
    $article->setAuthor($author);

このように ``by_reference=false`` が実際にすることは、親オブジェクトでセッターを呼ぶことをフレームワークに強制することです。

同様に、下層にあるコレクションに :doc:`collection</reference/forms/types/collection>` フォームタイプを利用している場合、データは（ Doctrine の ``ArrayCollection`` のように）オブジェクトです。そして、（例えば ``setAuthors`` のように）セッターが呼ばれる必要があるときは、\ ``by_reference`` は ``false`` に設定されるべきです。

.. 2014/01/16 yositani2002 50cd5c32a49376dd2b76b9a85cb7becc9967f79a
