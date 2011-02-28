フォームフィールド
===========

フォームは1つあるいは複数のフォームフィールドからなっています。それぞれのフィールドは :class:`Symfony\\Component\\Form\\FormFieldInterface` を実装したクラスのオブジェクトです。フィールドは標準化された表現と人間の読める表現の間で、データを変換します。

 ``DateField`` を例に見てみましょう。あなたのアプリケーションが日付を文字列あるいは ``DateTime`` オブジェクトとして保存する一方で、ユーザはドロップダウンで日付を選択したいとします。 ``DateField`` はレンダリングと型変換を担当します。

コアフィールドオプション
------------------

全てのビルトインされたフィールドは、それぞれのコンストラクタ内でオプションの配列を受け取ることができます。便宜上、これらのコアフィールドはいくつかのオプションが事前に定義された :class:`Symfony\\Component\\Form\\Field` のサブクラスになっています。

``data``
~~~~~~~~

フォームを作成する際、それぞれのフィールドは最初、フォームのドメインオブジェクトの対応するプロパティの値を表示します。これらの初期値を上書きしたい場合、 ``data`` オプションで設定ができます。

.. code-block:: php

    use Symfony\Component\Form\HiddenField
    
    $field = new HiddenField('token', array(
        'data' => 'abcdef',
    ));
    
    assert('abcdef' === $field->getData());

.. note::

     ``data`` オプションを使用した時、フィールドはドメインオブジェクトには書き込みを行いません。これは、 ``property_path`` オプションが暗黙的に ``null`` になるためです。詳しくは :ref:`form-field-property_path` を参照してください。

``required``
~~~~~~~~~~~~

デフォルトでは、それぞれの ``Field`` は値が必須であることが前提となっています。そのため、空の値は送信されません。この設定は動作といくつかのフィールドのレンダリングに影響を与えます。例えば ``ChoiceField`` は、必須でなければ空の選択肢を含みます。

.. code-block:: php

    use Symfony\Component\Form\ChoiceField
    
    $field = new ChoiceField('status', array(
        'choices' => array('tbd' => 'To be done', 'rdy' => 'Ready'),
        'required' => false,
    ));

``disabled``
~~~~~~~~~~~~

ユーザにフィールドの値を変更させたくない場合、 ``disabled`` オプションを ``true`` に設定することができます。あらゆる送信データは無視されます。

.. code-block:: php

    use Symfony\Component\Form\TextField
    
    $field = new TextField('status', array(
        'data' => 'Old data',
        'disabled' => true,
    ));
    $field->submit('New data');
    
    assert('Old data' === $field->getData());

``trim``
~~~~~~~~

入力フィールドの先頭や末尾にスペースをうっかり入れてしまうユーザがたくさんいます。フォームのフレームワークはこれらのスペースを自動的に削除します。もしスペースをそのままにしたいのなら、 ``trim`` オプションを ``false`` に設定してください。

.. code-block:: php

    use Symfony\Component\Form\TextField
    
    $field = new TextField('status', array(
        'trim' => false,
    ));
    $field->submit('   Data   ');
    
    assert('   Data   ' === $field->getData());

.. _form-field-property_path:

``property_path``
~~~~~~~~~~~~~~~~~

フィールドは、デフォルトでフォームのドメインオブジェクトのプロパティ値を表示します。フォームが送信された時、送信された値はオブジェクトに書き戻されます。

フィールドから読み出された、あるいはフィールドへ書き込んだプロパティを上書きしたい場合には、 ``property_path`` オプションを設定できます。このオプションのデフォルト値はフィールドの名前になっています。

.. code-block:: php

    use Symfony\Component\Form\Form
    use Symfony\Component\Form\TextField
    
    $author = new Author();
    $author->setFirstName('Your name...');
    
    $form = new Form('author');
    $form->add(new TextField('name', array(
        'property_path' => 'firstName',
    )));
    $form->bind($request, $author);
    
    assert('Your name...' === $form['name']->getData());

プロパティパスのために、ドメインオブジェクトのクラスに以下のいずれかが必要です。

1. 一致するパブリックなプロパティ、または
2. "set"または"get"から始まり、プロパティパスが続く、パブリックなセッタおよびゲッタ

プロパティパスは、ドット(.)を使うことで入れ子になったオブジェクトも参照できます。

.. code-block:: php

    use Symfony\Component\Form\Form
    use Symfony\Component\Form\TextField
    
    $author = new Author();
    $author->getEmail()->setAddress('me@example.com');
    
    $form = new Form('author');
    $form->add(new EmailField('email', array(
        'property_path' => 'email.address',
    )));
    $form->bind($request, $author);
    
    assert('me@example.com' === $form['email']->getData());

角括弧[]を使って、入れ子になった配列のエントリや ``\Traversable`` を実装したオブジェクトを参照できます。

.. code-block:: php

    use Symfony\Component\Form\Form
    use Symfony\Component\Form\TextField
    
    $author = new Author();
    $author->setEmails(array(0 => 'me@example.com'));
    
    $form = new Form('author');
    $form->add(new EmailField('email', array(
        'property_path' => 'emails[0]',
    )));
    $form->bind($request, $author);
    
    assert('me@example.com' === $form['email']->getData());

プロパティパスが ``null`` の場合、フィールドにはドメインオブジェクトからの読み書きはできません。これは、固定値を持つフィールドが欲しい場合に役に立ちます。

.. code-block:: php

    use Symfony\Component\Form\HiddenField
    
    $field = new HiddenField('token', array(
        'data' => 'abcdef',
        'property_path' => null,
    ));
    
``data`` オプションを設定した場合、``property_path`` は常に ``null`` になるのが一般的です。従って、前のコード例は以下のように単純にできます。

.. code-block:: php

    use Symfony\Component\Form\HiddenField
    
    $field = new HiddenField('token', array(
        'data' => 'abcdef',
    ));

.. note::

    カスタムのデフォルト値を設定したいけれどそのままドメインオブジェクトに書き込みたい場合は、 ``property_path`` を手動で渡す必要があります。

    .. code-block:: php

        use Symfony\Component\Form\TextField
        
        $field = new TextField('name', array(
            'data' => 'Custom default...',
            'property_path' => 'token',
        ));
    
    Usually this is not necessary, because you should rather the default value
    of the ``token`` property in your domain object.
    本来はドメインオブジェクト内で ``token`` プロパティのデフォルト値を決めるべきなので、通常はこういったことは必要になりません。

ビルトインフィールド
---------------

Symfony2は以下のフィールドを備えています。

.. toctree::
    :hidden:

    fields/index

.. include:: fields/map.rst.inc

